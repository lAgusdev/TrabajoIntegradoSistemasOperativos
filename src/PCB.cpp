#include "PCB.h"

class PCB{
    private:
        char id[MAXID];
        enum estado;
        int tiempo_llegada;
        int contador_programa;
        TlistRegistros registros_CPU;
        int tiempo_rafaga_restante;
        int tamaño_requerido;
        TlistPunMem punteros_memoria;
        TcolaOp operaciones_E_S;
        int tiempo_bloqueo;
        int tiempo_espera_acumulado;
        int tiempo_finalizacion;
    public:
        PCB(char in_Id[MAXID], int in_tiempo_llegada,int in_tiempo_rafaga_restante,int in_tamaño_requerido,TcolaOp in_operaciones_E_S);
        void carga_operaciones(TcolaOp * in_operaciones_E_S);
        void cambio_a_listo(TlistPunMem in_punteros_memoria);
        void cambio_a_ejecucion();
        void cambio_a_bloqueado(int tiempo_fin);
        void cambio_a_terminado(int tiempo_actual);
        RegistroE_S solicitar_proxima_ES();//Desencola y devuelve la siguiente operación de E/S de operaciones_E_S para que el módulo E/S la procese
        void acumular_espera(int tiempo);
        char* obtener_id();
        enum obtener_estado();
        int obtener_rafaga_restante();
        TlistPunMem obtener_punteros_memoria();
        int obtener_tiempo_bloqueo();

        void ejecutar_ciclo();
        TlistRegistros guardar_contexto():
        void restaurar_contexto();
        int ha_terminado();
};
PCB::PCB(char in_Id[MAXID], int in_tiempo_llegada,int in_tiempo_rafaga_restante,int in_tamaño_requerido,TcolaOp in_operaciones_E_S){
        strcpy(id,in_Id);
        estado=enum_estado.nuevo;
        tiempo_llegada=in_tiempo_llegada;
        tiempo_rafaga_restante=in_tiempo_rafaga_restante;
        tamaño_requerido=in_tamaño_requerido;

        PCB::carga_operaciones(&in_operaciones_E_S);

        registros_CPU=NULL;
        contador_programa=0;
        punteros_memoria=NULL;
        tiempo_bloqueo=0;
        tiempo_espera_acumulado=0;
        tiempo_finalizacion=0;

}
void PCB::cambio_a_listo(TlistPunMem in_punteros_memoria){
    punteros_memoria=in_punteros_memoria;
    estado=enum_estado.listo;
}
void PCB::carga_operaciones(TcolaOp * in_operaciones_E_S){
    TELEMENTOCOP elem;
    while(!vaciac(*in_operaciones_E_S)){
        sacac(in_operaciones_E_S,&elem);
        ponec(&operaciones_E_S,elem);
    }
}
void PCB::cambio_a_ejecucion(){
    estado=ejecucion;
}
void PCB::cambio_a_bloqueado(int tiempo_fin){
    estado=bloqueado;
    tiempo_bloqueo=tiempo_fin;
}
void PCB::cambio_a_terminado(int tiempo_actual){
    estado=terminado;
    tiempo_finalizacion=tiempo_actual;
}
RegistroE_S PCB::solicitar_proxima_ES(){
    TELEMENTOCOP elem;
    if(!vaciac(operaciones_E_S)){
        sacac(&operaciones_E_S,&elem);
        return elem;
    }else{
        return NULL;
    }
}
void PCB::acumular_espera(int tiempo){
    tiempo_espera_acumulado+=tiempo;
}
char* PCB::obtener_id(){
    return id;
}
enum PCB::obtener_estado(){
    return estado;
}
int PCB::obtener_rafaga_restante(){
    return tiempo_rafaga_restante;
}
TlistPunMem PCB::obtener_punteros_memoria(){
    return punteros_memoria;
}
int PCB::obtener_tiempo_bloqueo(){
    return tiempo_bloqueo;
}
void PCB::ejecutar_ciclo(){
    tiempo_rafaga_restante--;
    contador_programa++;
}
TlistRegistros PCB::guardar_contexto(){
    return registros_CPU;
}
void PCB::restaurar_contexto(){}
int PCB::ha_terminado(){
    if(tiempo_rafaga_restante==0){
        return 1;
    }else{
        return 0;
    }

}
PCB::~PCB(){
    //dtor
}
