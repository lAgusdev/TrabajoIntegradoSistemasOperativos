#ifndef DISPOSITIVOS_IO_H
#define DISPOSITIVOS_IO_H


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
#endif // DISPOSITIVOS_IO_H
