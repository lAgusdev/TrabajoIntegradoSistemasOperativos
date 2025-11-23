#include "Sistema_operativo.h"


Sistema_operativo::Sistema_operativo(config* conf) {
     reloj_global = new Reloj();
     unidad_central_proceso = new cpu();
     gestor_memoria = new memoria_prpal(256, 4096); // valores por defecto
     cola_listos = new ColaSincronizada(100); // capacidad 100
     planificador_largo_plazo = new memoria_sec();
     procesos_terminados = nullptr;
     dispositivos = nullptr;
}



bool Sistema_operativo::hay_trabajo_pendiente() {
    bool cpu_ocupada = !unidad_central_proceso->esta_libre();
    // TODO: implementar verificaciones completas
    return cpu_ocupada;
}

void Sistema_operativo::manejar_terminacion(PCB* pcb_terminado) {
    int tick_actual = reloj_global->obtener_tick();
    pcb_terminado->cambio_a_terminado(tick_actual);
    // TODO: convertir char* a int para liberar_memoria o cambiar firma
    // gestor_memoria->liberar_memoria(pcb_terminado->obtener_id());
    // TODO: usar lista enlazada correcta para procesos_terminados
}

void Sistema_operativo::planificar_largo_plazo() {
    // TODO: implementar cuando memoria_sec tenga métodos completos
}

void Sistema_operativo::planificar_corto_plazo() {
    if (unidad_central_proceso->esta_libre()) {
        // TODO: implementar cuando ColaSincronizada tenga métodos completos
        // PCB* pcb_listo = cola_listos->Retirar_de_Cola_Listos();
        // if (pcb_listo != nullptr) {
        //     unidad_central_proceso->asignar_proceso(pcb_listo);
        // }
    }
}

void Sistema_operativo::gestionar_desbloqueos() {
    // TODO: implementar cuando tengamos lista de dispositivos
}

void Sistema_operativo::ejecutar_simulacion() {
    // Simulación básica - expandir cuando todos los módulos estén completos
    for(int i = 0; i < 100; i++) {
        gestionar_desbloqueos();
        planificar_largo_plazo();
        if(!unidad_central_proceso->esta_libre()) {
            unidad_central_proceso->ejecutar_ciclo(nullptr);
        }
        planificar_corto_plazo();
        reloj_global->avanzar_tick();
    }
}
