#include "Sistema_operativo.h"
#include <cstdio>
#include <thread>
#include <chrono>


Sistema_operativo::Sistema_operativo(config* conf) {
     reloj_global = new Reloj();
     unidad_central_proceso = new cpu();
     gestor_memoria = new memoria_prpal(256, 4096);
     cola_listos = new ColaSincronizada(100);
     planificador_largo_plazo = new memoria_sec();
     procesos_terminados = nullptr;
     dispositivos = nullptr;
     tiempo_simulacion_ms = 100; // 100ms por ciclo de CPU
     std::cout<<"[SO] Sistema Operativo inicializado (Algoritmo: FIFO)"<<std::endl;
     std::cout<<"[SO] Tiempo por ciclo de CPU: "<<tiempo_simulacion_ms<<"ms"<<std::endl;
}

void Sistema_operativo::cargar_procesos_desde_archivo(const char* nombre_archivo){
    FILE* archivo = fopen(nombre_archivo, "rt");
    if(archivo == nullptr){
        std::cout<<"[SO] No se pudo abrir "<<nombre_archivo<<std::endl;
        return;
    }
    
    char id[MAXID];
    int tiempo_llegada, rafaga, tamanio;
    int procesos_cargados = 0;
    
    std::cout<<"[SO] Cargando procesos desde "<<nombre_archivo<<"..."<<std::endl;
    
    while(fscanf(archivo, "%[^,], %d, %d, %d\n", id, &tiempo_llegada, &rafaga, &tamanio) == 4){
        // Crear cola de operaciones E/S vacia (simplificado)
        TcolaOp operaciones_es;
        iniciac(operaciones_es);
        
        // Crear PCB
        PCB* nuevo_pcb = new PCB(id, tiempo_llegada, rafaga, tamanio, operaciones_es);
        
        // Agregar a memoria secundaria (cola de nuevos)
        planificador_largo_plazo->cargar_proceso(*nuevo_pcb);
        procesos_cargados++;
        
        std::cout<<"  - Proceso "<<id<<" cargado (llegada="<<tiempo_llegada<<", rafaga="<<rafaga<<")"<<std::endl;
    }
    
    fclose(archivo);
    std::cout<<"[SO] "<<procesos_cargados<<" procesos cargados en memoria secundaria"<<std::endl;
}

void Sistema_operativo::crear_proceso_interactivo(){
    char id[MAXID];
    int tiempo_llegada, rafaga, tamanio;
    int num_operaciones_es;
    
    std::cout<<"\n=== CREAR NUEVO PROCESO ==="<<std::endl;
    std::cout<<"ID del proceso (max "<<MAXID-1<<" caracteres): ";
    std::cin>>id;
    
    std::cout<<"Tiempo de llegada (ms): ";
    std::cin>>tiempo_llegada;
    
    std::cout<<"Tiempo de rafaga/CPU (ciclos): ";
    std::cin>>rafaga;
    
    std::cout<<"Tamanio requerido (marcos): ";
    std::cin>>tamanio;
    
    std::cout<<"Cantidad de operaciones de E/S: ";
    std::cin>>num_operaciones_es;
    
    // Crear cola de operaciones E/S
    TcolaOp operaciones_es;
    iniciac(operaciones_es);
    
    for(int i = 0; i < num_operaciones_es; i++){
        TELEMENTOCOP operacion;
        std::cout<<"\n  Operacion E/S #"<<(i+1)<<":"<<std::endl;
        std::cout<<"    Dispositivo (DISK/PRINTER/NETWORK): ";
        std::cin>>operacion.id_dispositivo;
        std::cout<<"    Duracion (ciclos): ";
        std::cin>>operacion.duracion;
        std::cout<<"    Tipo (0=lectura, 1=escritura): ";
        std::cin>>operacion.tipo_operacion;
        
        ponec(operaciones_es, operacion);
    }
    
    // Crear PCB y agregar a memoria secundaria
    PCB* nuevo_pcb = new PCB(id, tiempo_llegada, rafaga, tamanio, operaciones_es);
    planificador_largo_plazo->cargar_proceso(*nuevo_pcb);
    
    std::cout<<"\n[SO] Proceso "<<id<<" creado exitosamente!"<<std::endl;
    std::cout<<"[SO] El proceso sera admitido cuando haya memoria disponible."<<std::endl;
}

void Sistema_operativo::mostrar_estado_sistema(){
    std::cout<<"\n=== ESTADO DEL SISTEMA ==="<<std::endl;
    std::cout<<"Tick actual: "<<reloj_global->obtener_tick()<<std::endl;
    std::cout<<"CPU: "<<(unidad_central_proceso->esta_libre() ? "LIBRE" : "OCUPADA")<<std::endl;
    std::cout<<"Procesos en cola de listos: "<<(!cola_listos->vacia() ? "SI" : "NO")<<std::endl;
    std::cout<<"Procesos nuevos pendientes: "<<(planificador_largo_plazo->hay_procesos_pendientes() ? "SI" : "NO")<<std::endl;
    std::cout<<"Marcos de memoria libres: "<<gestor_memoria->obtener_marcos_libres()<<std::endl;
    std::cout<<"==========================="<<std::endl;
}

bool Sistema_operativo::hay_trabajo_pendiente() {
    bool cpu_ocupada = !unidad_central_proceso->esta_libre();
    bool listos_pendientes = !cola_listos->vacia();
    bool nuevos_pendientes = planificador_largo_plazo->hay_procesos_pendientes();
    
    return cpu_ocupada || listos_pendientes || nuevos_pendientes;
}

void Sistema_operativo::manejar_terminacion(PCB* pcb_terminado) {
    int tick_actual = reloj_global->obtener_tick();
    pcb_terminado->cambio_a_terminado(tick_actual);
    // TODO: convertir char* a int para liberar_memoria o cambiar firma
    // gestor_memoria->liberar_memoria(pcb_terminado->obtener_id());
    // TODO: usar lista enlazada correcta para procesos_terminados
}

void Sistema_operativo::planificar_largo_plazo() {
    if(planificador_largo_plazo->hay_procesos_pendientes()){
        PCB* pcb_nuevo = planificador_largo_plazo->obtener_proceso_para_admision();
        if(pcb_nuevo != nullptr){
            // Calcular marcos necesarios (simplificado: 1 marco por proceso)
            int marcos_necesarios = 1;
            
            if(gestor_memoria->obtener_marcos_libres() >= marcos_necesarios){
                std::cout<<"[Planificador LP] Admitiendo proceso "<<pcb_nuevo->obtener_id()<<" (marcos libres: "<<gestor_memoria->obtener_marcos_libres()<<")"<<std::endl;
                
                // Asignar ID numerico simple para el proceso
                int id_proceso = reloj_global->obtener_tick() % 1000;
                TlistPunMem punteros = gestor_memoria->asignar_memoria(id_proceso, marcos_necesarios);
                
                pcb_nuevo->cambio_a_listo(punteros);
                cola_listos->Aniadir_a_Cola_Listos(pcb_nuevo);
                
                // Eliminar de memoria secundaria
                char* id = pcb_nuevo->obtener_id();
                planificador_largo_plazo->eliminar_proceso(id);
            } else {
                std::cout<<"[Planificador LP] Memoria insuficiente, proceso en espera"<<std::endl;
            }
        }
    }
}

void Sistema_operativo::planificar_corto_plazo() {
    // FIFO: Solo asignar proceso si CPU está libre
    if(unidad_central_proceso->esta_libre() && !cola_listos->vacia()){
        PCB* pcb_listo = cola_listos->Retirar_de_Cola_Listos();
        if(pcb_listo != nullptr){
            std::cout<<"[Planificador CP - FIFO] Asignando proceso "<<pcb_listo->obtener_id()<<" a CPU"<<std::endl;
            unidad_central_proceso->asignar_proceso(pcb_listo);
        }
    }
}

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

void Sistema_operativo::ejecutar_simulacion() {
    std::cout<<"\n========== INICIO DE SIMULACION (FIFO) ==========\n"<<std::endl;
    int ciclo_limite = 100;
    
    while(hay_trabajo_pendiente() && reloj_global->obtener_tick() < ciclo_limite) {
        int tick_actual = reloj_global->obtener_tick();
        std::cout<<"\n--- Tick "<<tick_actual<<" ("<<(tick_actual * tiempo_simulacion_ms)<<"ms) ---"<<std::endl;
        
        // Simular tiempo real
        std::this_thread::sleep_for(std::chrono::milliseconds(tiempo_simulacion_ms));
        
        gestionar_desbloqueos();
        planificar_largo_plazo();
        
        // Verificar si hay proceso en ejecucion
        if(!unidad_central_proceso->esta_libre()) {
            PCB* proceso_actual = unidad_central_proceso->obtener_proceso_actual();
            
            // Verificar si el proceso termino
            if(proceso_actual->haterminado()){
                std::cout<<"[SO] Proceso "<<proceso_actual->obtener_id()<<" ha terminado"<<std::endl;
                proceso_actual->cambio_a_terminado(tick_actual);
                unidad_central_proceso->guardar_y_ceder(proceso_actual);
                manejar_terminacion(proceso_actual);
            }
            // FIFO: proceso continúa hasta terminar (no hay quantum)
            else {
                unidad_central_proceso->ejecutar_ciclo(dispositivos);
            }
        }
        
        planificar_corto_plazo();
        reloj_global->avanzar_tick();
    }
    
    std::cout<<"\n========== FIN DE SIMULACION (t="<<reloj_global->obtener_tick()<<") ==========\n"<<std::endl;
    gestor_memoria->mostrar_estado_memoria();
}
