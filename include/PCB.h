#ifndef PCB_H
#define PCB_H


class PCB{
    private:
        char id[MAXID];
        enum estado;
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
    public:
        PCB(char in_Id[MAXID], int in_tiempo_llegada,int in_tiempo_rafaga_restante,int in_tamanio_requerido,TcolaOp in_operaciones_E_S);
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

#endif // PCB_H
