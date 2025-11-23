#include "memoria_sec.h"
#include "PCB.h"
#include <iostream>

void memoria_sec::cargar_proceso(PCB& nuevo_pcb){
    TlistaPCB nuevo_nodo = new NodoPCB;
    nuevo_nodo->PCB = &nuevo_pcb;
    nuevo_nodo->sig = procesos_pendientes;
    procesos_pendientes = nuevo_nodo;
}
int memoria_sec::hay_procesos_pendientes(){
    return !(procesos_pendientes==nullptr);
}
PCB* memoria_sec::obtener_proceso_para_admision(){
    if(procesos_pendientes == nullptr) return nullptr;
    PCB* resultado = procesos_pendientes->PCB;
    return resultado;
}
void memoria_sec::eliminar_proceso(char id_proceso[]){
    if(procesos_pendientes == nullptr) return;
    TlistaPCB elim;
    elim=procesos_pendientes;
    procesos_pendientes=procesos_pendientes->sig;
    delete elim;
}
void memoria_sec::mostrar_lista(){
    TlistaPCB actlist=procesos_pendientes;
    std::cout<<"PROCESOS EN LA MEMORIA SECUNDARIA\n";
    while(actlist!=nullptr){
        std::cout<<actlist->PCB->obtener_id()<<std::endl;
        actlist=actlist->sig;
    }
}
