#include "memoria_prpal.h"


memoria_prpal::memoria_prpal(int cant_total_marcos, int tam_marco){
    tamanio_marco=tam_marco;
    char id_marco[MAXID];
    Tlistmarcos nuevo_list_marcos;
    while(cant_total_marcos>0){
        //necesita recibir el nombre de los marcos
        marco nuevo_marco=new marco(id_marco);

        nuevo_list_marcos=&nuevo_marco;
        nuevo_list_marcos->sig=marcos;
        marcos=nuevo_list_marcos;
        cant_total_marcos--;
    }

}
int memoria_prpal::obtener_marcos_libres(){
    int i=0;
    Tlistamarcos actmarco=marcos;
    while(actmarco!=NULL){
        if(actmarco->estaocupado==0){
            i++;
        }
        actmarco=actmarco->sig;
    }
    return i;
}
TlistPunMem memoria_prpal::asignar_memoria(int id_proceso, int marcos_requeridos){//requiere que se corrobore que se pueda asignar antes
    Tlistamarcos actmarco=marcos;
    TlistPunMem puntmem=NULL;
    while(actmarco->estaocupado){
        actmarco=actmarco->sig;
    }
    while(marcos_requeridos>0){
        marcos_requeridos--;
        TlistPunMem nuevo=new TlistPunMem;
        nuevo->marco->asignar(id_proceso);
        nuevo->sig=puntmem;
        puntmem=nuevo;
        actmarco=actmarco->sig;
    }
    return puntmem;
}
void memoria_prpal::liberar_memoria(int id_proceso){
     Tlistamarcos actmarco=marcos;
     while(actmarco!=NULL){
        if(strcmp(actmarc->obt_id_proceso_asignado(),id_proceso)){
            actmarco->liberar();
        }
        actmarco=actmarco->sig;
     }
}
void memoria_prpal::mostrar_estado_memoria(){
    Tlistamarcos actmarco=marcos;
    int i=1;
    cout<<"marco\t id_proceso_asignado\n";
    while(actmarco != NULL){
        cout<<i<<actmarco->obt_id_proceso_asignado()<<endl;
        actmarco=actmarco->sig;
        i++;
    }
}
memoria_prpal::~memoria_prpal()
{
    //dtor
}
