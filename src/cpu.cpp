#include "cpu.h"

class cpu{
private:
        PCB* proceso_actual;
        TlistRegistros registros_internos;
        int contador_programa_interno;
        int estado_libre;
public:
    CPU(Reloj* reloj_global);
    int esta_libre();
    void asignar_proceso(PCB* nuevo_pcb);
    void ejecutar_ciclo(DispositivosIO* io_manager);
    void guardar_y_ceder(PCB* pcb_saliente);
};

cpu::CPU(Reloj* reloj_global){
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
