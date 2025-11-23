#include <iostream>
#include "bios.h"
#include "config.h"
#include "Sistema_operativo.h"

using namespace std;

void menu(){
    char op1,op2;
    cout<<"Bienvenido a DoorOS";
    do{
        cout<<"Selecciones aplicacion: \n \t-a(Aplicacion1)\n \t-b(Aplicacion2)\n \t-c(Aplicacion3)\n \t-d(Aplicacion3)\n \t-e(Aplicacion4)\n";
        cin>>op1;
        switch(op1){
            case 'a':
                break;
            case 'b':
                break;
            case'c':
                break;
            case'd':
                break;
            case 'e':
                break;
        }
        cout<<"Quiere seguir utilizando el sistema:  s / n\n";
        cin>>op2;
    }while(op2!='n');
}

int main(){
    bios biosSys;
    tConfig config;
    Sistema_operativo doorOS;
    if(biosSys.compruebaHard(config))
        if(biosSys.cargaSO(config))
            if(doorOS.iniciaSO(config))
                menu();
    };
    return 0;
}
