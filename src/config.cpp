#include "config.h"

config::config(){
    FILE *configHard, *configDisIO;
    configHard = fopen("configHard.txt","rt");
    configDisIO = fopen("configDisIO.txt","rt");
    memoria_prpal memPSet;
    dispositivos_IO disIOSet, newDisIO;
    char nombre[MAXNOM];
    int cant_total_marcos, tam_marco, promedio;
    if(configHard != NULL){
        fscanf(configHard, "%d, %d", &cant_total_marcos, &tam_marco)
        memoria_prpal memPSet(cant_total_marcos, tam_marco);
        memP = memPSet;

        //Nose sabe que poner fscanf(configHard, "%d, %d", &cant_total_marcos, &tam_marco)
        memoria_sec memSSet();
        memS = memSSet;

        if(configDisIO != NULL){
            disIOSet = NULL
            while(fscanf(configDisIO, "%s, %d", nombre, &promedio) == 2){
                newDisIO = new dispositivos_IO(nombre, promedio);
                newDisIO->sig = disIOSet;
                disIOSet = newDisIO;
            };
            disIO = disIOSet;
            fclose(configDisIO);
        }
        fclose(configHard);
    };
};

int testCPU(){
    return
};

int testMemP(){
    return
};

int testMemS(){
    return
};

int testKeyboard(){
    while()
};

config::~config(){

    //dtor
}
