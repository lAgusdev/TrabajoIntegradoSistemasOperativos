#include "dispositivos_IO.h"
#include "PCB.h"
#include <cstring>
#include "string.h"

dispositivos_IO::dispositivos_IO(char nombre[], int tiempo_promedio){
    strcpy(nombre_dispositivo,nombre);
    estado=Estado_IO::LIBRE;
    iniciac(cola_espera);
    tiempo_promedio_operacion=tiempo_promedio;
}
void dispositivos_IO::solicitar_dispositivo(PCB* pcb_solicitante, int duracion_solicitada, int reloj_global){
    if(estado==Estado_IO::LIBRE){
        asignar_proceso_actual(pcb_solicitante,duracion_solicitada,reloj_global);
    }else{
        ponec(cola_espera,*pcb_solicitante);
    }
}
void dispositivos_IO::asignar_proceso_actual(PCB* pcb_asignar, int duracion_operacion, int reloj_global){
    estado=Estado_IO::OCUPADO;
    strcpy(id_proceso_asignado, pcb_asignar->obtener_id());
    tiempo_dasbloqueo=reloj_global+duracion_operacion;
}
char* dispositivos_IO::verificar_y_desbloquear(int reloj_global){
    if(tiempo_dasbloqueo==reloj_global){
            estado=Estado_IO::LIBRE;
            atender_siguiente(reloj_global);
            return id_proceso_asignado;
    }else{
        return nullptr;
    }
}
void dispositivos_IO::atender_siguiente(int reloj_global){
    TELEMENTOCPCB elem;
    if(!vaciac(cola_espera)){
        sacac(&cola_espera,&elem);
        asignar_proceso_actual(elem,elem->obtener_rafaga_restante(),reloj_global);
    }else{
        estado=Estado_IO::LIBRE;
    }
}
int dispositivos_IO::esta_libre(){
    return estado== Estado_IO::LIBRE;
}


