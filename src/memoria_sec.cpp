#include "memoria_sec.h"

void memoria_sec::cargar_proceso(PCB& nuevo_pcb){
    TlistaPCB ant,actlist=procesos_pendientes;
    while(act!=actlist){
        ant=actlist;
        actlist=actlist->sig;
    }
    ant->PCB=nuevo_pcb;
}
int memoria_sec::hay_procesos_pendientes(){
    return !(procesos_pendientes==NULL);
}
PCB* memoria_sec::obtener_proceso_para_admision(){
    eliminar_proceso(procesos_pendientes.obtener_id());
    return procesos_pendientes->PCB;
}
void memoria_sec::eliminar_proceso(char id_proceso[]){
    TListaPCB elim;
    elim=procesos_pendientes;
    procesos_pendientes=procesos_pendientes->sig;
    delete elim;
}
void memoria_sec::mostrar_lista(){
    TlistaPCB actlist=procesos_pendientes;
    cout<<"PROCESOS EN LA MEMORIA SECUNDARIA\n";
    while(actlist!=NULL){
        cout<<actlist->PCB->id<<endl;
    }
}
