#include "bios.h"
#include "Sistema_operativo.h"

void bios::compruebaHard(config &conf){
    if(conf.testCPU()){
        if(conf.testMemP()){
            if(conf.testMemS()){
                if(conf.testKeyboard())
                    std::cout<<"Hardware check passed"<<std::endl;
                else
                    std::cout<<"ERROR: Keyboard is not detected or has issues"<<std::endl;
            } else
                std::cout<<"ERROR: Secondary Memory is not detected or has issues"<<std::endl;
        } else
            std::cout<<"ERROR: Main Memory is not detected or has issues"<<std::endl;
    } else
        std::cout<<"ERROR: CPU is not detected or has issues"<<std::endl;
}

void bios::cargaSO(config &conf){
    std::cout<<"Searching in MBR SO"<<std::endl;
    std::cout<<"SO found"<<std::endl;
}
