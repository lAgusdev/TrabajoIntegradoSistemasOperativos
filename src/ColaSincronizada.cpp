#include "ColaSincronizada.h"

ColaSincronizada::ColaSincronizada(int capacidad) {
    capacidad_maxima = capacidad;
    // Inicializamos sem�foros:
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
    // 1. SEM�FORO: Esperamos que haya lugar (Decrementa vac�os)
    // Si la cola est� llena, esto bloquear�a al hilo productor.
    sem_vacios->wait();

    // 2. EXCLUSI�N MUTUA: Entramos a la secci�n cr�tica
    {
        std::lock_guard<std::mutex> lock(mutex_cola);
        cola_pcbs.push(proceso);
    } // El mutex se libera autom�ticamente aqu� al salir del scope

    // 3. SEM�FORO: Avisamos que hay un nuevo item (Incrementa items)
    sem_items->signal();
}

PCB* ColaSincronizada::Retirar_de_Cola_Listos() {
    // 1. SEM�FORO: Verificamos si hay items.
    // Usamos try_wait en lugar de wait porque tu simulaci�n es secuencial.
    // Si us�ramos wait() y la cola estuviera vac�a, el programa se congelar�a eternamente.
    if (!sem_items->try_wait()) {
        return nullptr; // No hab�a nada para consumir
    }

    PCB* proceso_retornado = nullptr;

    // 2. EXCLUSI�N MUTUA: Secci�n cr�tica
    {
        std::lock_guard<std::mutex> lock(mutex_cola);
        // Doble verificaci�n por seguridad (aunque el sem�foro ya nos garantiz� acceso)
        if (!cola_pcbs.empty()) {
            proceso_retornado = cola_pcbs.front();
            cola_pcbs.pop();
        }
    }

    // 3. SEM�FORO: Avisamos que se liber� un espacio (Incrementa vac�os)
    if (proceso_retornado != nullptr) {
        sem_vacios->signal();
    }

    return proceso_retornado;
}

// Verifica si la cola está vacía (thread-safe)
bool ColaSincronizada::vacia() {
    std::lock_guard<std::mutex> lock(mutex_cola);
    return cola_pcbs.empty();
}
