#include "Sistema_operativo.h"
#include <cstdio>
#include <thread>   // Para usar sleep (pausas reales)
#include <chrono>   // Para manejar milisegundos
#include <string>   // Para validación de dispositivos
#include <algorithm> // Para toupper

// CONSTRUCTOR: Inicializa todos los componentes del sistema operativo
Sistema_operativo::Sistema_operativo(config* conf) {
     reloj_global = new Reloj();                          // Reloj para contar el tiempo
     unidad_central_proceso = new cpu();                  // CPU que ejecuta procesos
     gestor_memoria = new memoria_prpal(256, 4096);       // Memoria con 256 marcos de 4096 bytes
     cola_listos = new ColaSincronizada(100);             // Cola de procesos listos (sincronizada)
     planificador_largo_plazo = new memoria_sec();        // Memoria secundaria (procesos nuevos)
     procesos_terminados = nullptr;                       // Lista de procesos terminados
     dispositivos = nullptr;                              // Dispositivos de E/S
     tiempo_simulacion_ms = 100;                          // Cada ciclo = 100ms reales
     std::cout<<"[SO] Sistema Operativo inicializado (Algoritmo: FIFO)"<<std::endl;
     std::cout<<"[SO] Tiempo por ciclo de CPU: "<<tiempo_simulacion_ms<<"ms"<<std::endl;
}

// CARGAR PROCESOS DESDE ARCHIVO: Lee procesos.txt y los carga en memoria secundaria
void Sistema_operativo::cargar_procesos_desde_archivo(const char* nombre_archivo){
    FILE* archivo = fopen(nombre_archivo, "rt");  // Abre el archivo en modo lectura
    if(archivo == nullptr){
        std::cout<<"[SO] No se pudo abrir "<<nombre_archivo<<std::endl;
        return;
    }
    
    char id[MAXID];                    // ID del proceso (nombre)
    int tiempo_llegada, rafaga, tamanio;  // Datos del proceso
    int procesos_cargados = 0;         // Contador de procesos
    
    std::cout<<"[SO] Cargando procesos desde "<<nombre_archivo<<"..."<<std::endl;
    
    // Lee línea por línea: ID, tiempo_llegada, rafaga, tamaño
    while(fscanf(archivo, "%[^,], %d, %d, %d\n", id, &tiempo_llegada, &rafaga, &tamanio) == 4){
        // Crear cola de operaciones E/S vacía
        TcolaOp operaciones_es;
        iniciac(operaciones_es);
        
        // Crear el PCB (Process Control Block) con los datos
        PCB* nuevo_pcb = new PCB(id, tiempo_llegada, rafaga, tamanio, operaciones_es);
        
        // Agregar a memoria secundaria (estado NUEVO)
        planificador_largo_plazo->cargar_proceso(*nuevo_pcb);
        procesos_cargados++;
        
        std::cout<<"  - Proceso "<<id<<" cargado (llegada="<<tiempo_llegada<<", rafaga="<<rafaga<<")"<<std::endl;
    }
    
    fclose(archivo);
    std::cout<<"[SO] "<<procesos_cargados<<" procesos cargados en memoria secundaria"<<std::endl;
}

// CREAR PROCESO INTERACTIVO: Permite al usuario crear un proceso manualmente
void Sistema_operativo::crear_proceso_interactivo(){
    char id[MAXID];                   // ID del proceso
    int tiempo_llegada, rafaga, tamanio;  // Datos del proceso
    int num_operaciones_es;           // Cuántas operaciones de E/S tendrá
    
    // Solicitar datos al usuario CON VALIDACIÓN
    std::cout<<"\n=== CREAR NUEVO PROCESO ==="<<std::endl;
    std::cout<<"ID del proceso (max "<<MAXID-1<<" caracteres): ";
    std::cin>>id;
    
    // VALIDAR: Tiempo de llegada >= 0
    do {
        std::cout<<"Tiempo de llegada (ms, >= 0): ";
        std::cin>>tiempo_llegada;
        if(std::cin.fail() || tiempo_llegada < 0){
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout<<"  [ERROR] Ingrese un numero valido >= 0"<<std::endl;
            tiempo_llegada = -1;
        }
    } while(tiempo_llegada < 0);
    
    // VALIDAR: Ráfaga > 0
    do {
        std::cout<<"Tiempo de rafaga/CPU (ciclos, > 0): ";
        std::cin>>rafaga;
        if(std::cin.fail() || rafaga <= 0){
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout<<"  [ERROR] Ingrese un numero valido > 0"<<std::endl;
            rafaga = 0;
        }
    } while(rafaga <= 0);
    
    // VALIDAR: Tamaño entre 1 y 256 (marcos disponibles)
    do {
        std::cout<<"Tamanio requerido (marcos, 1-256): ";
        std::cin>>tamanio;
        if(std::cin.fail() || tamanio < 1 || tamanio > 256){
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout<<"  [ERROR] Ingrese un numero entre 1 y 256"<<std::endl;
            tamanio = 0;
        }
    } while(tamanio < 1 || tamanio > 256);
    
    // VALIDAR: Operaciones E/S >= 0
    do {
        std::cout<<"Cantidad de operaciones de E/S (>= 0): ";
        std::cin>>num_operaciones_es;
        if(std::cin.fail() || num_operaciones_es < 0){
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout<<"  [ERROR] Ingrese un numero valido >= 0"<<std::endl;
            num_operaciones_es = -1;
        }
    } while(num_operaciones_es < 0);
    
    // Crear cola de operaciones E/S
    TcolaOp operaciones_es;
    iniciac(operaciones_es);
    
    // Solicitar datos de cada operación E/S CON VALIDACIÓN
    for(int i = 0; i < num_operaciones_es; i++){
        TELEMENTOCOP operacion;
        std::cout<<"\n  Operacion E/S #"<<(i+1)<<":"<<std::endl;
        
        // VALIDAR: Dispositivo válido (DISCO, IMPRESORA, RED)
        bool dispositivo_valido = false;
        do {
            std::cout<<"    Dispositivo (DISCO/IMPRESORA/RED): ";
            std::cin>>operacion.id_dispositivo;
            
            // Convertir a mayúsculas para validar
            std::string disp_upper = operacion.id_dispositivo;
            for(auto &c : disp_upper) c = toupper(c);
            
            if(disp_upper == "DISCO" || disp_upper == "IMPRESORA" || disp_upper == "RED"){
                dispositivo_valido = true;
            } else {
                std::cout<<"      [ERROR] Dispositivo invalido. Use: DISCO, IMPRESORA o RED"<<std::endl;
            }
        } while(!dispositivo_valido);
        
        // VALIDAR: Duración > 0
        do {
            std::cout<<"    Duracion (ciclos, > 0): ";
            std::cin>>operacion.duracion;
            if(std::cin.fail() || operacion.duracion <= 0){
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout<<"      [ERROR] Ingrese un numero valido > 0"<<std::endl;
                operacion.duracion = 0;
            }
        } while(operacion.duracion <= 0);
        
        // VALIDAR: Tipo 0 o 1
        do {
            std::cout<<"    Tipo (0=lectura, 1=escritura): ";
            std::cin>>operacion.tipo_operacion;
            if(std::cin.fail() || (operacion.tipo_operacion != 0 && operacion.tipo_operacion != 1)){
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout<<"      [ERROR] Ingrese 0 (lectura) o 1 (escritura)"<<std::endl;
                operacion.tipo_operacion = -1;
            }
        } while(operacion.tipo_operacion != 0 && operacion.tipo_operacion != 1);
        
        ponec(operaciones_es, operacion);  // Agregar a la cola
    }
    
    // Crear PCB con todos los datos y agregarlo a memoria secundaria
    PCB* nuevo_pcb = new PCB(id, tiempo_llegada, rafaga, tamanio, operaciones_es);
    planificador_largo_plazo->cargar_proceso(*nuevo_pcb);
    
    std::cout<<"\n[SO] Proceso "<<id<<" creado exitosamente!"<<std::endl;
    std::cout<<"[SO] El proceso sera admitido cuando haya memoria disponible."<<std::endl;
}

// MOSTRAR ESTADO DEL SISTEMA: Muestra información actual del sistema
void Sistema_operativo::mostrar_estado_sistema(){
    std::cout<<"\n=== ESTADO DEL SISTEMA ==="<<std::endl;
    std::cout<<"Tick actual: "<<reloj_global->obtener_tick()<<std::endl;
    std::cout<<"CPU: "<<(unidad_central_proceso->esta_libre() ? "LIBRE" : "OCUPADA")<<std::endl;
    std::cout<<"Procesos en cola de listos: "<<(!cola_listos->vacia() ? "SI" : "NO")<<std::endl;
    std::cout<<"Procesos nuevos pendientes: "<<(planificador_largo_plazo->hay_procesos_pendientes() ? "SI" : "NO")<<std::endl;
    std::cout<<"Marcos de memoria libres: "<<gestor_memoria->obtener_marcos_libres()<<std::endl;
    std::cout<<"==========================="<<std::endl;
}

// Verifica si hay trabajo pendiente (procesos por ejecutar)
bool Sistema_operativo::hay_trabajo_pendiente() {
    bool cpu_ocupada = !unidad_central_proceso->esta_libre();        // ¿CPU está ejecutando algo?
    bool listos_pendientes = !cola_listos->vacia();                  // ¿Hay procesos esperando?
    bool nuevos_pendientes = planificador_largo_plazo->hay_procesos_pendientes();  // ¿Hay procesos nuevos?
    
    return cpu_ocupada || listos_pendientes || nuevos_pendientes;
}

// Maneja un proceso que terminó su ejecución
void Sistema_operativo::manejar_terminacion(PCB* pcb_terminado) {
    int tick_actual = reloj_global->obtener_tick();
    pcb_terminado->cambio_a_terminado(tick_actual);  // Cambia estado a TERMINADO
    
    // LIBERAR MEMORIA: Obtener el ID numérico y liberar los marcos
    int id_memoria = pcb_terminado->obtener_id_proceso_memoria();
    if(id_memoria != -1){
        std::cout<<"[SO] Liberando memoria del proceso "<<pcb_terminado->obtener_id()<<" (ID memoria: "<<id_memoria<<")"<<std::endl;
        gestor_memoria->liberar_memoria(id_memoria);
    }
    
    // TODO: usar lista enlazada correcta para procesos_terminados
}

// PLANIFICADOR DE LARGO PLAZO: Decide qué procesos NUEVOS pasan a LISTOS
void Sistema_operativo::planificar_largo_plazo() {
    // Si hay procesos nuevos esperando...
    if(planificador_largo_plazo->hay_procesos_pendientes()){
        PCB* pcb_nuevo = planificador_largo_plazo->obtener_proceso_para_admision();
        if(pcb_nuevo != nullptr){
            // Calcular marcos necesarios (simplificado: 1 marco por proceso)
            int marcos_necesarios = 1;
            
            // ¿Hay memoria disponible?
            if(gestor_memoria->obtener_marcos_libres() >= marcos_necesarios){
                std::cout<<"[Planificador LP] Admitiendo proceso "<<pcb_nuevo->obtener_id()<<" (marcos libres: "<<gestor_memoria->obtener_marcos_libres()<<")"<<std::endl;
                
                // Asignar ID numérico simple para el proceso
                int id_proceso = reloj_global->obtener_tick() % 1000;
                pcb_nuevo->asignar_id_proceso_memoria(id_proceso);  // GUARDAR el ID en el PCB
                TlistPunMem punteros = gestor_memoria->asignar_memoria(id_proceso, marcos_necesarios);
                
                // Cambiar de NUEVO → LISTO y agregarlo a la cola
                pcb_nuevo->cambio_a_listo(punteros);
                cola_listos->Aniadir_a_Cola_Listos(pcb_nuevo);
                
                // Eliminar de memoria secundaria (ya fue admitido)
                char* id = pcb_nuevo->obtener_id();
                planificador_largo_plazo->eliminar_proceso(id);
            } else {
                std::cout<<"[Planificador LP] Memoria insuficiente, proceso en espera"<<std::endl;
            }
        }
    }
}

// PLANIFICADOR DE CORTO PLAZO (FIFO): Decide qué proceso LISTO pasa a EJECUCIÓN
void Sistema_operativo::planificar_corto_plazo() {
    // FIFO: Solo asignar proceso si CPU está libre
    if(unidad_central_proceso->esta_libre() && !cola_listos->vacia()){
        PCB* pcb_listo = cola_listos->Retirar_de_Cola_Listos();  // Saca el primero de la cola
        if(pcb_listo != nullptr){
            std::cout<<"[Planificador CP - FIFO] Asignando proceso "<<pcb_listo->obtener_id()<<" a CPU"<<std::endl;
            unidad_central_proceso->asignar_proceso(pcb_listo);  // LISTO → EJECUCIÓN
        }
    }
}

// Gestionar desbloqueos de procesos que esperaban E/S
void Sistema_operativo::gestionar_desbloqueos() {
    // Simplificado: verificar dispositivo único si existe
    if(dispositivos != nullptr){
        int tick = reloj_global->obtener_tick();
        char* id_desbloqueado = dispositivos->verificar_y_desbloquear(tick);
        
        if(id_desbloqueado != nullptr){
            std::cout<<"[Gestor E/S] Proceso "<<id_desbloqueado<<" desbloqueado en t="<<tick<<std::endl;
            // Buscar PCB y mover a listos (simplificado, asumimos que dispositivo maneja esto)
        }
    }
}

// EJECUTAR SIMULACIÓN: Bucle principal que ejecuta todo el sistema operativo
void Sistema_operativo::ejecutar_simulacion() {
    std::cout<<"\n========== INICIO DE SIMULACION (FIFO) ==========\n"<<std::endl;
    int ciclo_limite = 100;  // Máximo 100 ticks para evitar loops infinitos
    
    // Bucle principal: mientras haya trabajo y no supere el límite
    while(hay_trabajo_pendiente() && reloj_global->obtener_tick() < ciclo_limite) {
        int tick_actual = reloj_global->obtener_tick();
        std::cout<<"\n--- Tick "<<tick_actual<<" ("<<(tick_actual * tiempo_simulacion_ms)<<"ms) ---"<<std::endl;
        
        // Simular tiempo real: pausa de 100ms por cada ciclo
        std::this_thread::sleep_for(std::chrono::milliseconds(tiempo_simulacion_ms));
        
        // 1. Verificar si hay procesos bloqueados que ya terminaron E/S
        gestionar_desbloqueos();
        
        // 2. Planificador de largo plazo: NUEVO → LISTO (admisión a memoria)
        planificar_largo_plazo();
        
        // 3. Verificar si hay proceso en ejecución
        if(!unidad_central_proceso->esta_libre()) {
            PCB* proceso_actual = unidad_central_proceso->obtener_proceso_actual();
            
            // ¿El proceso terminó su ráfaga?
            if(proceso_actual->haterminado()){
                std::cout<<"[SO] Proceso "<<proceso_actual->obtener_id()<<" ha terminado"<<std::endl;
                proceso_actual->cambio_a_terminado(tick_actual);
                unidad_central_proceso->guardar_y_ceder(proceso_actual);  // Libera la CPU
                manejar_terminacion(proceso_actual);
            }
            // FIFO: proceso continúa hasta terminar (no hay quantum ni preempción)
            else {
                unidad_central_proceso->ejecutar_ciclo(dispositivos);  // Ejecuta 1 ciclo
            }
        }
        
        // 4. Planificador de corto plazo: LISTO → EJECUCIÓN (asignar a CPU)
        planificar_corto_plazo();
        
        // 5. Avanzar el reloj global
        reloj_global->avanzar_tick();
    }
    
    std::cout<<"\n========== FIN DE SIMULACION (t="<<reloj_global->obtener_tick()<<") ==========\n"<<std::endl;
    gestor_memoria->mostrar_estado_memoria();
}
