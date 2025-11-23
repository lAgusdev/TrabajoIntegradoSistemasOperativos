#include "Sistema_operativo.h"
#include <cstdio>


Sistema_operativo::Sistema_operativo(config* conf) {
     reloj_global = new Reloj();
     unidad_central_proceso = new cpu();
     gestor_memoria = new memoria_prpal(256, 4096);
     cola_listos = new ColaSincronizada(100);
     planificador_largo_plazo = new memoria_sec();
     procesos_terminados = nullptr;
     dispositivos = nullptr;
     quantum = 3; // Quantum de 3 ciclos para Round Robin
     ciclos_actuales = 0;
     std::cout<<"[SO] Sistema Operativo inicializado (Quantum="<<quantum<<")"<<std::endl;
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
    // Verificar si proceso actual termino o agoto quantum
    if(!unidad_central_proceso->esta_libre()){
        // Hay proceso ejecutando, verificar condiciones de desalojo
        return; // Dejamos que ejecute hasta terminar o pedir E/S
    }
    
    // CPU libre, asignar siguiente proceso de cola de listos
    if(!cola_listos->vacia()){
        PCB* pcb_listo = cola_listos->Retirar_de_Cola_Listos();
        if(pcb_listo != nullptr){
            std::cout<<"[Planificador CP] Asignando proceso "<<pcb_listo->obtener_id()<<" a CPU"<<std::endl;
            unidad_central_proceso->asignar_proceso(pcb_listo);
            ciclos_actuales = 0;
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
    std::cout<<"\n========== INICIO DE SIMULACION ==========\n"<<std::endl;
    int ciclo_limite = 50;
    
    while(hay_trabajo_pendiente() && reloj_global->obtener_tick() < ciclo_limite) {
        int tick_actual = reloj_global->obtener_tick();
        std::cout<<"\n--- Tick "<<tick_actual<<" ---"<<std::endl;
        
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
                ciclos_actuales = 0;
            }
            // Verificar si agoto el quantum (Round Robin)
            else if(ciclos_actuales >= quantum){
                std::cout<<"[SO] Quantum agotado para proceso "<<proceso_actual->obtener_id()<<", cediendo CPU"<<std::endl;
                unidad_central_proceso->guardar_y_ceder(proceso_actual);
                // Cambiar a estado LISTO y devolver a cola
                std::cout<<"[PCB "<<proceso_actual->obtener_id()<<"] Transicion: EJECUCION -> LISTO (fin de quantum)"<<std::endl;
                cola_listos->Aniadir_a_Cola_Listos(proceso_actual);
                ciclos_actuales = 0;
            }
            else {
                // Continuar ejecutando
                unidad_central_proceso->ejecutar_ciclo(dispositivos);
                ciclos_actuales++;
            }
        }
        
        planificar_corto_plazo();
        reloj_global->avanzar_tick();
    }
    
    std::cout<<"\n========== FIN DE SIMULACION (t="<<reloj_global->obtener_tick()<<") ==========\n"<<std::endl;
    gestor_memoria->mostrar_estado_memoria();
}
