#ifndef BUFFER_COMPARTIDO_H
#define BUFFER_COMPARTIDO_H

typedef struct{
    int estado;
}Tsemaforo;


class buffer_compartido{
        Tlistbuffer buffer;
        Tsemaforo sem_mutex;
        Tsemaforo sem_vacios;
        Tsemaforo sem_llenos;
    public:

};

#endif // BUFFER_COMPARTIDO_H
