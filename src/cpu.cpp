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
PCB* cpu::obtener_proceso_actual(){
    return proceso_actual;
}
void cpu::asignar_proceso(PCB* nuevo_pcb){
    if(nuevo_pcb != nullptr){
        nuevo_pcb->cambio_a_ejecucion();
        nuevo_pcb->restaurar_contexto();
        proceso_actual = nuevo_pcb;
        estado_libre = 0;
        std::cout<<"[CPU] Proceso "<<nuevo_pcb->obtener_id()<<" asignado a CPU"<<std::endl;
    }
}
void cpu::ejecutar_ciclo(dispositivos_IO* io_manager){
    if(proceso_actual != nullptr){
        proceso_actual->ejecutar_ciclo();
        std::cout<<"[CPU] Ejecutando proceso "<<proceso_actual->obtener_id()<<" (rafaga restante: "<<proceso_actual->obtener_rafaga_restante()<<")"<<std::endl;
    }
}
void cpu::guardar_y_ceder(PCB* pcb_saliente){
    if(pcb_saliente != nullptr){
        registros_internos = pcb_saliente->guardar_contexto();
        std::cout<<"[CPU] Contexto guardado para proceso "<<pcb_saliente->obtener_id()<<std::endl;
    }
    proceso_actual = nullptr;
    estado_libre = 1;
}
