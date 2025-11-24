#include <iostream>
#include "bios.h"
#include "config.h"
#include "Sistema_operativo.h"

using namespace std;

// Función que muestra el menú principal con las opciones disponibles
void mostrar_menu_principal(){
    cout<<"\n========================================="<<endl;
    cout<<"     DOOROS - Sistema Operativo"<<endl;
    cout<<"   Algoritmo: FIFO (First In First Out)"<<endl;
    cout<<"========================================="<<endl;
    cout<<"1. Crear proceso manualmente"<<endl;
    cout<<"2. Cargar procesos desde archivo"<<endl;
    cout<<"3. Iniciar simulacion"<<endl;
    cout<<"4. Ver estado del sistema"<<endl;
    cout<<"5. Reiniciar sistema"<<endl;
    cout<<"6. Salir"<<endl;
    cout<<"========================================="<<endl;
    cout<<"Seleccione una opcion: ";
}

int main(){
    // Crear objetos del BIOS y configuración del sistema
    bios biosSys;
    config conf;
    
    // Iniciar el BIOS: comprueba hardware y carga el sistema operativo
    cout<<"Iniciando BIOS..."<<endl;
    biosSys.compruebaHard(conf);  // Verifica que el hardware esté OK
    biosSys.cargaSO(conf);        // Carga el sistema operativo
    
    int opcion;
    bool reiniciar = true;  // Controla si se debe reiniciar el sistema
    
    // Bucle principal: permite reiniciar el sistema completo
    while(reiniciar){
        // Crear el sistema operativo con la configuración (cada vez que se reinicia)
        Sistema_operativo doorOS(&conf);
        bool simulacion_ejecutada = false;  // Bandera para controlar si ya se ejecutó la simulación
        
        // Bucle del menú
        do {
            mostrar_menu_principal();  // Muestra las opciones
            cin>>opcion;               // Lee la opción del usuario
            
            // VALIDAR: Limpiar buffer si la entrada falló
            if(cin.fail()){
                cin.clear();
                cin.ignore(10000, '\n');
                opcion = -1;  // Forzar opción inválida
            }
            
            switch(opcion){
                case 1: {
                    // OPCIÓN 1: Crear un proceso manualmente
                    // Permite definir ID, tiempo de llegada, ráfaga, memoria y operaciones E/S
                    doorOS.crear_proceso_interactivo();
                    break;
                }
                case 2: {
                    // OPCIÓN 2: Cargar procesos desde un archivo de texto
                    // Lee procesos desde archivos como procesos.txt
                    char nombre_archivo[100];
                    cout<<"Ingrese nombre del archivo (ej: procesos.txt): ";
                    cin>>nombre_archivo;
                    doorOS.cargar_procesos_desde_archivo(nombre_archivo);
                    break;
                }
                case 3: {
                    // OPCIÓN 3: Iniciar la simulación del sistema operativo
                    if(simulacion_ejecutada){
                        cout<<"\n[AVISO] La simulacion ya fue ejecutada."<<endl;
                        cout<<"Use la opcion 5 (Reiniciar sistema) para ejecutar otra simulacion."<<endl;
                    } else {
                        doorOS.ejecutar_simulacion();  // Ejecuta todos los procesos con FIFO
                        simulacion_ejecutada = true;   // Marca que ya se ejecutó
                    }
                    break;
                }
                case 4: {
                    // OPCIÓN 4: Ver el estado actual del sistema
                    // Muestra cuántos procesos hay en cada estado (NUEVO, LISTO, etc.)
                    doorOS.mostrar_estado_sistema();
                    break;
                }
                case 5: {
                    // OPCIÓN 5: Reiniciar el sistema operativo
                    cout<<"\n[SO] Reiniciando sistema operativo..."<<endl;
                    cout<<"[SO] Liberando recursos..."<<endl;
                    opcion = -2;  // Código especial para reiniciar
                    break;
                }
                case 6: {
                    // OPCIÓN 6: Salir del programa
                    cout<<"\nSaliendo del sistema..."<<endl;
                    reiniciar = false;  // No reiniciar, salir completamente
                    break;
                }
                default:
                    // Si el usuario ingresa una opción inválida
                    cout<<"\nOpcion invalida. Intente nuevamente."<<endl;
            }
            
            // Pausa después de cada operación (excepto al salir o reiniciar)
            if(opcion != 6 && opcion != -2){
                cout<<"\nPresione Enter para continuar...";
                cin.ignore();  // Limpia el buffer
                cin.get();     // Espera Enter
            }
            
        } while(opcion != 6 && opcion != -2);  // Repite hasta salir o reiniciar
    }
    
    cout<<"Simulacion completada. Adios!"<<endl;
    return 0;
}
