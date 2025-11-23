#include <iostream>
#include "bios.h"
#include "config.h"
#include "Sistema_operativo.h"

using namespace std;

void mostrar_menu_principal(){
    cout<<"\n========================================="<<endl;
    cout<<"     DOOROS - Sistema Operativo"<<endl;
    cout<<"   Algoritmo: FIFO (First In First Out)"<<endl;
    cout<<"========================================="<<endl;
    cout<<"1. Crear proceso manualmente"<<endl;
    cout<<"2. Cargar procesos desde archivo"<<endl;
    cout<<"3. Iniciar simulacion"<<endl;
    cout<<"4. Ver estado del sistema"<<endl;
    cout<<"5. Salir"<<endl;
    cout<<"========================================="<<endl;
    cout<<"Seleccione una opcion: ";
}

int main(){
    bios biosSys;
    config conf;
    
    cout<<"Iniciando BIOS..."<<endl;
    biosSys.compruebaHard(conf);
    biosSys.cargaSO(conf);
    
    Sistema_operativo doorOS(&conf);
    
    int opcion;
    bool simulacion_ejecutada = false;
    
    do {
        mostrar_menu_principal();
        cin>>opcion;
        
        switch(opcion){
            case 1: {
                doorOS.crear_proceso_interactivo();
                break;
            }
            case 2: {
                char nombre_archivo[100];
                cout<<"Ingrese nombre del archivo (ej: procesos.txt): ";
                cin>>nombre_archivo;
                doorOS.cargar_procesos_desde_archivo(nombre_archivo);
                break;
            }
            case 3: {
                if(simulacion_ejecutada){
                    cout<<"\n[AVISO] La simulacion ya fue ejecutada."<<endl;
                    cout<<"Reinicie el programa para ejecutar otra simulacion."<<endl;
                } else {
                    doorOS.ejecutar_simulacion();
                    simulacion_ejecutada = true;
                }
                break;
            }
            case 4: {
                doorOS.mostrar_estado_sistema();
                break;
            }
            case 5: {
                cout<<"\nSaliendo del sistema..."<<endl;
                break;
            }
            default:
                cout<<"\nOpcion invalida. Intente nuevamente."<<endl;
        }
        
        if(opcion != 5){
            cout<<"\nPresione Enter para continuar...";
            cin.ignore();
            cin.get();
        }
        
    } while(opcion != 5);
    
    cout<<"Simulacion completada. Adios!"<<endl;
    return 0;
}
