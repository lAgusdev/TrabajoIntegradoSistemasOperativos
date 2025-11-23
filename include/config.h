#ifndef CONFIG_H
#define CONFIG_H
#include "cpu.h"
#include "memoria_prpal.h"
#include "memoria_sec.h"
#include "dispositivos_IO.h"

class config{
    cpu CPU;
    memoria_prpal memP;
    memoria_sec memS;
    dispositivos_IO disIO;
    public:
        config();
        int testCPU();
        int testMemP();
        int testMemS();
        int testKeyboard();
};

#endif // CONFIG_H
