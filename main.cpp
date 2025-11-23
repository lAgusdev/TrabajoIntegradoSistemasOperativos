#include <iostream>
using namespace std;

void menu(){
    char op1,op2;
    compruebaHard();
    cargaSO();
    iniciaSO();
    cout<<"Bienvenido a NewOS";
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
        cout<<"quiere seguir utilizando el sistema:  s / n\n";
        cin>>op2;
    }while(op2!='n');
}

int main(){
    menu();
    return 0;
}
