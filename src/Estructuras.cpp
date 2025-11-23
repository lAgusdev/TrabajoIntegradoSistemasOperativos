#include "Estructuras.h"

// ==========================================================================
// IMPLEMENTACIÓN DE FUNCIONES DE COLA (TcolaOp)
// ==========================================================================

// Inicializa la cola poniendo los punteros a null
void iniciac(TcolaOp& c) {
    c.frente = nullptr;
    c.final = nullptr;
}

// Devuelve 1 (true) si la cola está vacía, 0 (false) si tiene elementos
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
        // Si estaba vacía, el nuevo es tanto el frente como el final
        c.frente = nuevo;
    } else {
        // Si ya había elementos, lo enganchamos al último
        c.final->sig = nuevo;
    }

    // 3. Actualizar el puntero final
    c.final = nuevo;
}

// Saca un elemento del frente de la cola
void sacac(TcolaOp* c, TELEMENTOCOP* dato) {
    // Protección básica contra cola vacía
    if (c->frente == nullptr) {
        return;
    }

    // 1. Guardar referencia al nodo que vamos a eliminar
    NodoColaOp* aux = c->frente;

    // 2. Extraer el dato
    *dato = aux->dato;

    // 3. Avanzar el frente
    c->frente = aux->sig;

    // 4. Si la cola quedó vacía, actualizar también el final
    if (c->frente == nullptr) {
        c->final = nullptr;
    }

    // 5. Liberar la memoria del nodo extraído
    delete aux;
}
// ==========================================================================
// IMPLEMENTACIÓN DE COLA DE OPERACIONES (TcolaOp)
// ==========================================================================
void iniciac(TcolaOp& c) {
    c.frente = nullptr;
    c.final = nullptr;
}

int vaciac(TcolaOp c) {
    return (c.frente == nullptr);
}

void ponec(TcolaOp& c, TELEMENTOCOP dato) {
    NodoColaOp* nuevo = new NodoColaOp;
    nuevo->dato = dato;
    nuevo->sig = nullptr;
    if (c.final == nullptr) c.frente = nuevo;
    else c.final->sig = nuevo;
    c.final = nuevo;
}

void sacac(TcolaOp* c, TELEMENTOCOP* dato) {
    if (c->frente == nullptr) return;
    NodoColaOp* aux = c->frente;
    *dato = aux->dato;
    c->frente = aux->sig;
    if (c->frente == nullptr) c->final = nullptr;
    delete aux;
}

// ==========================================================================
// [NUEVO] IMPLEMENTACIÓN DE COLA DE PCBs (TcolaPcb)
// ==========================================================================

void iniciac(TcolaPcb& c) {
    c.frente = nullptr;
    c.final = nullptr;
}

int vaciac(TcolaPcb c) {
    return (c.frente == nullptr);
}

// Versión 1: Recibe PCB& (referencia) -> Guarda su dirección
// Esto soluciona tu llamada: ponec(cola_espera, *pcb_solicitante);
void ponec(TcolaPcb& c, PCB& dato) {
    NodoPCB* nuevo = new NodoPCB;
    nuevo->PCB = &dato; // Guardamos la dirección del objeto original
    nuevo->sig = nullptr;

    if (c.final == nullptr) c.frente = nuevo;
    else c.final->sig = nuevo;
    c.final = nuevo;
}

// Versión 2: Recibe PCB* (puntero) -> Guarda el puntero directo
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

    // NOTA: No hacemos 'delete aux->PCB' porque borraríamos el proceso real.
    // Solo borramos el nodo de la cola.
    delete aux;
}
