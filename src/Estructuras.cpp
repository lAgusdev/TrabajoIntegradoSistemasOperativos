#include "Estructuras.h"

// ==========================================================================
// IMPLEMENTACI�N DE FUNCIONES DE COLA (TcolaOp)
// ==========================================================================

// Inicializa la cola poniendo los punteros a null
void iniciac(TcolaOp& c) {
    c.frente = nullptr;
    c.final = nullptr;
}

// Devuelve 1 (true) si la cola est� vac�a, 0 (false) si tiene elementos
int vaciac(TcolaOp c) {
    return (c.frente == nullptr);
}

// Agrega un elemento al final de la cola
void ponec(TcolaOp& c, TELEMENTOCOP dato) {
    // 1. Crear el nuevo nodo
    NodoColaOp* nuevo = new NodoColaOp;
    nuevo->dato = dato;
    nuevo->sig = nullptr;

    // 2. Enlazarlo
    if (c.final == nullptr) {
        // Si estaba vac�a, el nuevo es tanto el frente como el final
        c.frente = nuevo;
    } else {
        // Si ya hab�a elementos, lo enganchamos al �ltimo
        c.final->sig = nuevo;
    }

    // 3. Actualizar el puntero final
    c.final = nuevo;
}

// Saca un elemento del frente de la cola
void sacac(TcolaOp* c, TELEMENTOCOP* dato) {
    // Protecci�n b�sica contra cola vac�a
    if (c->frente == nullptr) {
        return;
    }

    // 1. Guardar referencia al nodo que vamos a eliminar
    NodoColaOp* aux = c->frente;

    // 2. Extraer el dato
    *dato = aux->dato;

    // 3. Avanzar el frente
    c->frente = aux->sig;

    // 4. Si la cola qued� vac�a, actualizar tambi�n el final
    if (c->frente == nullptr) {
        c->final = nullptr;
    }

    // 5. Liberar la memoria del nodo extra�do
    delete aux;
}

// ==========================================================================
// [NUEVO] IMPLEMENTACI�N DE COLA DE PCBs (TcolaPcb)
// ==========================================================================

void iniciac(TcolaPcb& c) {
    c.frente = nullptr;
    c.final = nullptr;
}

int vaciac(TcolaPcb c) {
    return (c.frente == nullptr);
}

// Versi�n 1: Recibe PCB& (referencia) -> Guarda su direcci�n
// Esto soluciona tu llamada: ponec(cola_espera, *pcb_solicitante);
void ponec(TcolaPcb& c, PCB& dato) {
    NodoPCB* nuevo = new NodoPCB;
    nuevo->PCB = &dato; // Guardamos la direcci�n del objeto original
    nuevo->sig = nullptr;

    if (c.final == nullptr) c.frente = nuevo;
    else c.final->sig = nuevo;
    c.final = nuevo;
}

// Versi�n 2: Recibe PCB* (puntero) -> Guarda el puntero directo
void ponec(TcolaPcb& c, PCB* dato) {
    if (dato == nullptr) return;
    NodoPCB* nuevo = new NodoPCB;
    nuevo->PCB = dato;
    nuevo->sig = nullptr;

    if (c.final == nullptr) c.frente = nuevo;
    else c.final->sig = nuevo;
    c.final = nuevo;
}

void sacac(TcolaPcb* c, TELEMENTOCPCB* dato) {
    if (c->frente == nullptr) {
        *dato = nullptr;
        return;
    }
    NodoPCB* aux = c->frente;
    *dato = aux->PCB; // Retornamos el puntero al PCB

    c->frente = aux->sig;
    if (c->frente == nullptr) c->final = nullptr;

    // NOTA: No hacemos 'delete aux->PCB' porque borrar�amos el proceso real.
    // Solo borramos el nodo de la cola.
    delete aux;
}
