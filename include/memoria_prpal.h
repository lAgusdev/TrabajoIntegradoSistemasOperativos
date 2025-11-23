#ifndef MEMORIA_PRPAL_H
#define MEMORIA_PRPAL_H
#include "marco.h"
#include "PCB.h"
typedef struct nodomarc{
    marco * marco;
    struct nodomarc *sig;
}Tnodomarc;
typedef Tnodomarc *Tlistmarcos;


class memoria_prpal{
private:
    int tamanio_marco;
    Tlistmarcos marcos;
public:
    memoria_prpal(int cant_total_marcos, int tam_marco);
    int obtener_marcos_libres();
    TlistPunMem asignar_memoria(int id_proceso, int marcos_requeridos);
    void liberar_memoria(int id_proceso);
    void mostrar_estado_memoria();
};

#endif // MEMORIA_PRPAL_H
