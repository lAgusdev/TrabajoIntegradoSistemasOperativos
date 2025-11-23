#include "Reloj.h"

Reloj::Reloj() : tick_actual(0) {
}

Reloj::~Reloj() {
}

int Reloj::obtener_tick() {
    return tick_actual;
}

void Reloj::avanzar_tick() {
    tick_actual++;
}
