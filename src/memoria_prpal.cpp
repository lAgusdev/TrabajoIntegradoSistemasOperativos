#include "memoria_prpal.h"
#include <cstdio>
#include <cstring>
#include <iostream>

memoria_prpal::memoria_prpal(int cant_total_marcos, int tam_marco){
    tamanio_marco=tam_marco;
    marcos = nullptr;
    char id_marco[MAXID];
    for(int i = 0; i < cant_total_marcos; i++){
        sprintf(id_marco, "M%d", i);
        marco* nuevo_marco = new marco(id_marco);
        Tlistmarcos nuevo_nodo = new NodoMarco;
        nuevo_nodo->contenido = nuevo_marco;
        nuevo_nodo->sig = marcos;
        marcos = nuevo_nodo;
    }
}
int memoria_prpal::obtener_marcos_libres(){
    int i=0;
    Tlistmarcos actmarco=marcos;
    while(actmarco!=nullptr){
        if(!actmarco->contenido->esta_ocupado()){
            i++;
        }
        actmarco=actmarco->sig;
    }
    return i;
}
TlistPunMem memoria_prpal::asignar_memoria(int id_proceso, int marcos_requeridos){//requiere que se corrobore que se pueda asignar antes
    Tlistmarcos actmarco=marcos;
    TlistPunMem puntmem=nullptr;
    while(actmarco!=nullptr && actmarco->contenido->esta_ocupado()){
        actmarco=actmarco->sig;
    }
    while(marcos_requeridos>0 && actmarco!=nullptr){
        marcos_requeridos--;
        TlistPunMem nuevo=new NodoPunMem;
        actmarco->contenido->asignar(id_proceso);
        nuevo->marco_asignado = actmarco->contenido;
        nuevo->sig=puntmem;
        puntmem=nuevo;
        actmarco=actmarco->sig;
        while(actmarco!=nullptr && actmarco->contenido->esta_ocupado()){
            actmarco=actmarco->sig;
        }
    }
    return puntmem;
}
void memoria_prpal::liberar_memoria(int id_proceso){
     Tlistmarcos actmarco=marcos;
     while(actmarco!=nullptr){
        if(actmarco->contenido->obt_id_proceso_asignado() == id_proceso){
            actmarco->contenido->liberar();
        }
        actmarco=actmarco->sig;
     }
}
void memoria_prpal::mostrar_estado_memoria(){
    Tlistmarcos actmarco=marcos;
    int i=1;
    std::cout<<"marco\t id_proceso_asignado\n";
    while(actmarco != nullptr){
        std::cout<<i<<"\t"<<actmarco->contenido->obt_id_proceso_asignado()<<std::endl;
        actmarco=actmarco->sig;
        i++;
    }
}
