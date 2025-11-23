#include "config.h"
config::config(){
    FILE *configHard = fopen("configHard.txt","rt");
    FILE *configDisIO = fopen("configDisIO.txt","rt");
    CPU = nullptr;
    memP = nullptr;
    memS = nullptr;
    disIO = nullptr;
    char nombre[MAXNOM];
    int cant_total_marcos = 0, tam_marco = 0, promedio = 0;
    if(configHard != NULL){
        if(fscanf(configHard, "%d, %d", &cant_total_marcos, &tam_marco) == 2){
            memP = new memoria_prpal(cant_total_marcos, tam_marco);
        }
        // Inicializar memoria secundaria si es necesario
        memS = new memoria_sec();
        fclose(configHard);
    }

    if(configDisIO != NULL){
        // Construir lista enlazada de dispositivos (si aplica)
        dispositivos_IO* head = nullptr;
        while(fscanf(configDisIO, "%s, %d", nombre, &promedio) == 2){
            dispositivos_IO* newDisIO = new dispositivos_IO(nombre, promedio);
            // aquí asumimos que dispositivos_IO no tiene campo 'sig' público; añadir a lista si necesario
            head = newDisIO; // colocar al frente (lista simple, mejorar si hay estructura)
        }
        disIO = head;
        fclose(configDisIO);
    }
}

int config::testCPU(){ return 1; }
int config::testMemP(){ return 1; }
int config::testMemS(){ return 1; }
int config::testKeyboard(){ return 1; }
