#include "marco.h"
#include <string>
#include <cstring>

marco::marco(char* id_m){
    strcpy(id, id_m);
    Ocupado=0;
    id_proceso_asignado_int = -1;
}
int marco::esta_ocupado(){
    return Ocupado;
}
void marco::asignar(int nuevo_id_proceso){
     Ocupado=1;
     id_proceso_asignado_int = nuevo_id_proceso;
}
void marco::liberar(){
    Ocupado=0;
    id_proceso_asignado_int = -1;
}
int marco::obt_id_proceso_asignado(){
    return id_proceso_asignado_int;
}
