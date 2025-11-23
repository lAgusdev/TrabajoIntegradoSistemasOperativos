#ifndef CPU_H
#define CPU_H
#include "PCB.h"

class cpu{
private:
    PCB* proceso_actual;
    TlistRegistros registros_internos;
    int contador_programa_interno;
    int estado_libre;
public:
    cpu(Reloj* reloj_global);
    int esta_libre();
    void asignar_proceso(PCB* nuevo_pcb);
    void ejecutar_ciclo(DispositivosIO* io_manager);
    void guardar_y_ceder(PCB* pcb_saliente);
};

#endif // CPU_H
