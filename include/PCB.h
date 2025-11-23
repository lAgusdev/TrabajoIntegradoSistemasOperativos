#ifndef PCB_H
#define PCB_H
#include "estado.h"
#include "Estructuras.h"
#include <iostream>
class PCB{
    private:
        char id[MAXID];
        estado  Estado;
        int tiempo_llegada;
        int contador_programa;
        TlistRegistros registros_CPU;
        int tiempo_rafaga_restante;
        int tamanio_requerido;
        TlistPunMem punteros_memoria;
        TcolaOp operaciones_E_S;
        int tiempo_bloqueo;
        int tiempo_espera_acumulado;
        int tiempo_finalizacion;
        int id_proceso_memoria;  // ID numérico usado para asignar memoria
    public:
        PCB(char in_Id[MAXID], int in_tiempo_llegada,int in_tiempo_rafaga_restante,int in_tamanio_requerido,TcolaOp in_operaciones_E_S);
        void carga_operaciones(TcolaOp * in_operaciones_E_S);
        void cambio_a_listo(TlistPunMem in_punteros_memoria);
        void cambio_a_ejecucion();
        void cambio_a_bloqueado(int tiempo_fin);
        void cambio_a_terminado(int tiempo_actual);
        RegistroE_S solicitar_proxima_ES();
        void acumular_espera(int tiempo);
        char* obtener_id();
        estado obtener_estado();
        int obtener_rafaga_restante();
        TlistPunMem obtener_punteros_memoria();
        int obtener_tiempo_bloqueo();
        int obtener_id_proceso_memoria();  // Obtener ID numérico de memoria
        void asignar_id_proceso_memoria(int id);  // Asignar ID numérico de memoria

        void ejecutar_ciclo();
        TlistRegistros guardar_contexto();
        void restaurar_contexto();
        int haterminado();
};

#endif // PCB_H
