#ifndef RELOJ_H
#define RELOJ_H
#include <iostream>

class Reloj
{
    private:
        int tick_actual;
    public:
        Reloj();
        virtual ~Reloj();
        int obtener_tick();
        void avanzar_tick();
};

#endif // RELOJ_H
