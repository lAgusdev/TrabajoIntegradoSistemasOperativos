#include "dispositivos_IO.h"

class dispositivos_IO{
private:
    char nombre_dispositivo[MAXNOM];
    enum estado;
    TcolaPcb cola_espera;
    char id_proceso_asignado[MAXID];
    int tiempo_dasbloqueo;
    int tiempo_promedio_operacion;
public:
    dispositivos_IO(char nombre[], int tiempo_promedio);
    void solicitar_dispositivo(PCB* pcb_solicitante, int duracion_solicitada, int reloj_global);
    void asignar_proceso_actual(PCB* pcb_asignar, int duracion_operacion, int reloj_global);
    char* verificar_y_desbloquear(int reloj_global);
    void atender_siguiente(int reloj_global);
    int esta_libre();
};
dispositivos_IO::dispositivos_IO(char nombre[], int tiempo_promedio){
    strcpy(nombre_dispositivo,nombre);
    estado=LIBRE;
    iniciac(cola_espera);
    tiempo_promedio_operacion=tiempo_promedio;
}
void dispositivos_IO::solicitar_dispositivo(PCB* pcb_solicitante, int duracion_solicitada, int reloj_global){
    if(estado==LIBRE){
        asignar_proceso_actual(pcb_solicitante,duracion_solicitada,reloj_global);
    }else{
        ponec(cola_espera,*pcb_solicitante);
    }
}
void dispositivos_IO::asignar_proceso_actual(PCB* pcb_asignar, int duracion_operacion, int reloj_global){
    estado=OCUPADO;
    id_proceso_asignado=PCB->obtener_id;
    tiempo_dasbloqueo)reloj_global+duracion_operacion;
}
char* dispositivos_IO::verificar_y_desbloquear(int reloj_global){
    if(tiempo_dasbloqueo==reloj_global){
            estado=LIBRE;
            return id_proceso_asignado;
            atender_siguiente(reloj_global);
    }
}
void dispositivos_IO::atender_siguiente(int reloj_global){
    TELEMENTOCPCB elem;
    if(!vaciac(cola_espera)){
        sacac(&cola_espera,&elem);
        asignar_proceso_actual(elem,elem.obt_tiempo_rafaga_restante,reloj_global);
    }else{
        estado="LIBRE";
    }
}
int dispositivos_IO::esta_libre(){
    return estado=="LIBRE";
}


dispositivos_IO::~dispositivos_IO()
{
    //dtor
}
