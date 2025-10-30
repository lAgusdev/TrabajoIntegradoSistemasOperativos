#ifndef MEMORIA_RAM_H
#define MEMORIA_RAM_H

typedef struct{
    int id_proceso; //Identificador del proceso que actualmente ocupa este marco de memoria.
    int numero_pagina; //La página específica del proceso que está cargada en este marco.
    int libre; //Indicador rápido de si el marco está disponible
    int bit_modificado; //Indica si el contenido del marco fue modificado desde que se cargó
    int bit_referencia; //Indica si el marco fue accedido recientemente.
    int tiempo_carga; //El ciclo de reloj del SO en el que este marco fue cargado
}TdatoMarco;

typedef struct nodomarco{
    TdatoMarco dato;
    struct  nodomarco*sig;
}TnodoMarco;

typedef TnodoMarco *Tlistamarcos;


class memoria_ram{
        int tamanio_total;
        int marco_tamanio;
        Tlistamarcos marcos;
        Tlistamarcos marcoslibres;
    public:
        memoria_ram(int new_tam_total,int new_tamanio_marco);
};

    memoria_ram::memoria_ram(int new_tam_total,int new_tamanio_marco){
        tamanio_total=new_tam_total;
        marco_tamanio=new_tamanio_marco;
        marcos=NULL;
        marcoslibres=NULL;
    }
#endif // MEMORIA_RAM_H
