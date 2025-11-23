#ifndef MARCO_H
#define MARCO_H
#define MAXID 7
#include <string.h>

#include "PCB.h"
class marco{
    char id[MAXID];
    char id_proeso_asignado[MAXID];
    int Ocupado;
public:
    marco(int id_m);
    int esta_ocupado();
    void asignar(char nuevo_id_proceso[]);
    void liberar();
    char* obtener_id_proceso_asignado();
};
#endif // MARCO_H
