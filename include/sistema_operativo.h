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

class Sistema_operativo{
    private:
        Reloj* reloj_global;
        CPU* unidad_central_proceso;
        memoria_prpal* gestor_memoria;
        memoria_sec* planificador_largo_plazo;
        TLista<dispositivos_IO>* dispositivos;
        ColaSincronizada* cola_listos;
        TListaPCB* procesos_terminados;
    public:
        Sistema_operativo(Config* conf);
        void ejecutar_simulacion();
        void planificar_largo_plazo();
        void planificar_corto_plazo();
        void gestionar_desbloqueos();
        void manejar_terminacion(PCB* pcb_terminado);
};

#endif // SISTEMA_OPERATIVO_H
