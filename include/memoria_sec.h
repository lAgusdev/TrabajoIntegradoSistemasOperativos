#ifndef MEMORIA_SEC_H
#define MEMORIA_SEC_H
#include "PCB.h"

class memoria_sec{
    TListaPCB procesos_pendientes;
    int capacidad_simulada;
public:
    void cargar_proceso(PCB& nuevo_pcb);
    int hay_procesos_pendientes();
    PCB* obtener_proceso_para_admision();
    void eliminar_proceso(char id_proceso[]);
    void mostrar_lista();

};

#endif // MEMORIA_SEC_H
