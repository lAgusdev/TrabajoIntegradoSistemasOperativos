#include "ColaSincronizada.h"

ColaSincronizada::ColaSincronizada(int capacidad) {
    capacidad_maxima = capacidad;
    // Inicializamos semáforos:
    // sem_vacios: inicia con TOTAL de capacidad (buffer libre)
    sem_vacios = new Semaforo(capacidad);
    // sem_items: inicia en 0 (no hay nada que consumir al inicio)
    sem_items = new Semaforo(0);
}

ColaSincronizada::~ColaSincronizada() {
    delete sem_vacios;
    delete sem_items;
}

void ColaSincronizada::Aniadir_a_Cola_Listos(PCB* proceso) {
    // 1. SEMÁFORO: Esperamos que haya lugar (Decrementa vacíos)
    // Si la cola está llena, esto bloquearía al hilo productor.
    sem_vacios->wait();

    // 2. EXCLUSIÓN MUTUA: Entramos a la sección crítica
    {
        std::lock_guard<std::mutex> lock(mutex_cola);
        cola_pcbs.push(proceso);
    } // El mutex se libera automáticamente aquí al salir del scope

    // 3. SEMÁFORO: Avisamos que hay un nuevo item (Incrementa items)
    sem_items->signal();
}

PCB* ColaSincronizada::Retirar_de_Cola_Listos() {
    // 1. SEMÁFORO: Verificamos si hay items.
    // Usamos try_wait en lugar de wait porque tu simulación es secuencial.
    // Si usáramos wait() y la cola estuviera vacía, el programa se congelaría eternamente.
    if (!sem_items->try_wait()) {
        return nullptr; // No había nada para consumir
    }

    PCB* proceso_retornado = nullptr;

    // 2. EXCLUSIÓN MUTUA: Sección crítica
    {
        std::lock_guard<std::mutex> lock(mutex_cola);
        // Doble verificación por seguridad (aunque el semáforo ya nos garantizó acceso)
        if (!cola_pcbs.empty()) {
            proceso_retornado = cola_pcbs.front();
            cola_pcbs.pop();
        }
    }

    // 3. SEMÁFORO: Avisamos que se liberó un espacio (Incrementa vacíos)
    if (proceso_retornado != nullptr) {
        sem_vacios->signal();
    }

    return proceso_retornado;
}

bool ColaSincronizada::vacia() {
    std::lock_guard<std::mutex> lock(mutex_cola);
    return cola_pcbs.empty();
}
