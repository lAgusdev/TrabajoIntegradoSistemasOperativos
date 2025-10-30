#include <iostream>

using namespace std;
#define MAXREG

void menu(){
    char op1,op2;
    do{
        cout<<"seleccione una opcion:\n";
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
