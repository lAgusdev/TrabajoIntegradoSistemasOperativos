#include "cpu.h"



cpu::cpu(Reloj* reloj_global){
    proceso_actual=NULL;
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
void cpu::ejecutar_ciclo(dispositivos_IO* io_manager,Reloj* reloj_global){
    proceso_actual->ejecutar_ciclo();
    io_manager->verificar_y_desbloquear(reloj_global);
}
void cpu::guardar_y_ceder(PCB* pcb_saliente){
    pcb_saliente->guardar_contexto();
}
cpu::~cpu()
{
    //dtor
}
