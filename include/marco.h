#ifndef MARCO_H
#define MARCO_H
#include <string.h>
#include "Estructuras.h"
#include <iostream>

class marco{
    char id[MAXID];
    int id_proceso_asignado_int;
    int Ocupado;
public:
    marco(char* id_m);
    int esta_ocupado();
    void asignar(int nuevo_id_proceso);
    void liberar();
    int obt_id_proceso_asignado();
};
#endif // MARCO_H
