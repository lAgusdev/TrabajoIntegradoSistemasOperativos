#ifndef SISTEMA_OPERATIVO_H
#define SISTEMA_OPERATIVO_H
#include <iostream>
#include "Reloj.h"
#include "cpu.h"
#include "memoria_prpal.h"
#include "memoria_sec.h"
#include "dispositivos_IO.h"
#include "ColaSincronizada.h"
#include "PCB.h"
#include "config.h"

class Sistema_operativo{
    private:
        Reloj* reloj_global;
        cpu* unidad_central_proceso;
        memoria_prpal* gestor_memoria;
        memoria_sec* planificador_largo_plazo;
        dispositivos_IO* dispositivos; // Simplificado, expandir si se necesita lista
        ColaSincronizada* cola_listos;
        TlistaPCB procesos_terminados;
        int quantum;
        int ciclos_actuales;
    public:
        Sistema_operativo(config* conf);
        void ejecutar_simulacion();
        void planificar_largo_plazo();
        void planificar_corto_plazo();
        void gestionar_desbloqueos();
        void manejar_terminacion(PCB* pcb_terminado);
        bool hay_trabajo_pendiente();
        void cargar_procesos_desde_archivo(const char* nombre_archivo);
};

#endif // SISTEMA_OPERATIVO_H
