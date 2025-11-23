#include "bios.h"
#include "Sistema_operativo.h"
#include "config.h"

int compruebaHard(tConfig config){
    if(testCPU())
        if(testMemP())
            if(testMemS())
                if(testKeyboard())
                    return 1;
                else
                    cout<<"ERROR: Keyboard is not detected has issues";
            else
                count<<"ERROR: Secundary Memory is not detected has issues";
        else
            cout<<"ERROR: Main Memory is not detected has issues";
    else
        cout<<"ERROR: CPU is not detected or has issues";
};

void cargaSO(tConfig config){
    cout<<"Searching in MBR SO";
    cout<<"SO found";
    s
};
