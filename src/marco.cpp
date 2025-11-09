#include "marco.h"

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
marco::marco(int id_m){
    strcpy(id,id_m);
    Ocupado=0;
    id_proeso_asignado="LIBRE";
}
int marco::esta_ocupado(){
    return Ocupado;
}
void marco::asignar(char nuevo_id_proceso[]){
     Ocupado=1;
     strcpy(id_proeso_asignado,nuevo_id_proceso);
}
void marco::liberar(){
    Ocupado=0;
    strcpy(id_proeso_asignado,"LIBRE");
}
char* marco::obtener_id_proceso_asignado(){
    return id_proeso_asignado;
}
marco::~marco()
{
    //dtor
}
