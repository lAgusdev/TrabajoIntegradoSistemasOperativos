#include "cpu.h"
#include "PCB.h"


cpu::cpu(){
    proceso_actual=nullptr;
    contador_programa_interno =0;
    estado_libre=1;
}
int cpu::esta_libre(){
    return estado_libre;
}
void cpu::asignar_proceso(PCB* nuevo_pcb){
    nuevo_pcb->restaurar_contexto();
    proceso_actual = nuevo_pcb;
    estado_libre = 0;
}
void cpu::ejecutar_ciclo(dispositivos_IO* io_manager){
    if(proceso_actual != nullptr){
        proceso_actual->ejecutar_ciclo();
    }
}
void cpu::guardar_y_ceder(PCB* pcb_saliente){
    pcb_saliente->guardar_contexto();
}
