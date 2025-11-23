#include "config.h"

config::config(){
    FILE *configHard, *configDisIO;
    configHard = fopen("configHard.txt","rt");
    configDisIO = fopen("configDisIO.txt","rt");
    cpu CPUSet;
    memoria_prpal memPSet;
    memoria_sec memSSet;
    dispositivos_IO disIOSet, newDisIO;
    char nombre[MAXNOM];
    int promedio;
    if(configHard != NULL){
        cpu CPUSet();
        CPU = CPUSet;


        memoria_prpal memPSet();
        memP = memPSet;



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
