#ifndef CPU_H
#define CPU_H
#include <iostream>
#include "PCB.h"
#include "Reloj.h"
#include "dispositivos_IO.h"
class cpu{
private:
    PCB* proceso_actual;
    TlistRegistros registros_internos;
    int contador_programa_interno;
    int estado_libre;
public:
    cpu();
    int esta_libre();
    PCB* obtener_proceso_actual();
    void asignar_proceso(PCB* nuevo_pcb);
    void ejecutar_ciclo(dispositivos_IO* io_manager);
    void guardar_y_ceder(PCB* pcb_saliente);
};

#endif // CPU_H
