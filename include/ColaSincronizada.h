#ifndef COLASINCRONIZADA_H
#define COLASINCRONIZADA_H


#include <queue>
#include <mutex>
#include <condition_variable>
#include "PCB.h"

class Semaforo {
private:
    std::mutex mtx;
    std::condition_variable cv;
    int cuenta;

public:
    explicit Semaforo(int cuenta_inicial = 0) : cuenta(cuenta_inicial) {}

    // P() o wait(): Decrementa. Bloquea si es 0.
    void wait() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return cuenta > 0; });
        cuenta--;
    }

    // try_wait(): Intenta decrementar. Retorna false si es 0 (no bloquea).
    // Útil para evitar deadlocks en simulaciones de un solo hilo.
    bool try_wait() {
        std::lock_guard<std::mutex> lock(mtx);
        if (cuenta > 0) {
            cuenta--;
            return true;
        }
        return false;
    }

    // V() o signal(): Incrementa y despierta hilos.
    void signal() {
        std::unique_lock<std::mutex> lock(mtx);
        cuenta++;
        cv.notify_one();
    }
};

// --- Clase Principal: Cola Sincronizada ---
class ColaSincronizada {
private:
    std::queue<PCB*> cola_pcbs;     // Contenedor subyacente
    int capacidad_maxima;           // Tamaño del buffer

    std::mutex mutex_cola;          // Exclusión mutua para tocar la cola
    Semaforo* sem_vacios;           // Productor: Cuenta espacios libres
    Semaforo* sem_items;            // Consumidor: Cuenta items disponibles

public:
    // Constructor: Define el tamaño del buffer
    ColaSincronizada(int capacidad);

    // Destructor
    ~ColaSincronizada();

    // Productor: Añade un PCB (bloquea si está llena)
    void Aniadir_a_Cola_Listos(PCB* proceso);


    PCB* Retirar_de_Cola_Listos();

    // Verifica si está vacía (thread-safe)
    bool vacia();
};


#endif // COLASINCRONIZADA_H
