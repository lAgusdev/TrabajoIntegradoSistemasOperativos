#include "Sistema_operativo.h"


Sistema_operativo::Sistema_operativo(Config* conf) {
     reloj_global = new Reloj();
     unidad_central_proceso = new CPU(this);
     gestor_memoria = new memoria_prpal(conf->total_marcos, conf->tam_marco);
     cola_listos = new ColaSincronizada(conf->tamanio_buffer);
     planificador_largo_plazo = new memoria_sec();


     for (int i = 0; i < conf->num_dispositivos; ++i) {
         //cargardispositivos
    }
}



bool Sistema_operativo::hay_trabajo_pendiente() {
    bool cpu_ocupada = !unidad_central_proceso->esta_libre();
    bool listos_pendientes = !cola_listos->vacia();
    bool nuevos_pendientes = planificador_largo_plazo->hay_procesos_pendientes();
    bool bloqueados_pendientes = false;
    for (dispositivos_IO* dev : lista_dispositivos_IO) {
        if (dev->esta_ocupado_o_con_cola()) { bloqueados_pendientes = true; break; }
    }

    return cpu_ocupada || listos_pendientes || nuevos_pendientes || bloqueados_pendientes;
}

void Sistema_operativo::manejar_terminacion(PCB* pcb_terminado) {
    int tick_actual = reloj_global->obtener_tick();
    pcb_terminado->cambio_a_terminado(tick_actual);
    gestor_memoria->liberar_memoria(pcb_terminado->obtener_id());
    procesos_terminados.push_back(pcb_terminado);
}

void Sistema_operativo::planificar_largo_plazo() {
    if (planificador_largo_plazo->hay_procesos_pendientes()) {
        PCB* pcb_nuevo = planificador_largo_plazo->obtener_proceso_para_admision();
        int marcos_requeridos = 0;

        TlistPunMem punteros = gestor_memoria->asignar_memoria(pcb_nuevo->obtener_id(), marcos_requeridos);

        if (!punteros.vacia()) {
            pcb_nuevo->cambio_a_listo(punteros);
            cola_listos->Aniadir_a_Cola_Listos(pcb_nuevo);
            planificador_largo_plazo->eliminar_proceso(pcb_nuevo->obtener_id());
        }
    }
}

void Sistema_operativo::planificar_corto_plazo() {
    if (unidad_central_proceso->esta_libre()) {

        PCB* pcb_listo = cola_listos->Retirar_de_Cola_Listos();

        if (pcb_listo != nullptr) {
            unidad_central_proceso->asignar_proceso(pcb_listo);
        }
    }
}

void Sistema_operativo::gestionar_desbloqueos() {
    int tick = reloj_global->obtener_tick();

    for (dispositivos_IO* dev : lista_dispositivos_IO) {
        PCB* pcb_desbloqueado = dev->verificar_y_desbloquear(tick);

        if (pcb_desbloqueado != nullptr) {
            cola_listos->Aniadir_a_Cola_Listos(pcb_desbloqueado);
        }
    }
}

void Sistema_operativo::ejecutar_simulacion() {
    while (hay_trabajo_pendiente()) {

        gestionar_desbloqueos();

        planificar_largo_plazo();

        unidad_central_proceso->ejecutar_ciclo(lista_dispositivos_IO, reloj_global->obtener_tick());

        planificar_corto_plazo();

        reloj_global->avanzar_tick();
    }
}
