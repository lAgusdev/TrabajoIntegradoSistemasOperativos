#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

#include <iostream>
#include <cstring>

// ==========================================================================
// 1. FORWARD DECLARATIONS (Evitan ciclos de inclusión)
// ==========================================================================
// Esto permite declarar punteros a estas clases sin incluir sus .h aquí
class PCB;
class marco;

// ==========================================================================
// 2. CONSTANTES Y TIPOS BÁSICOS
// ==========================================================================
#define MAXID 20

// ==========================================================================
// 3. COLA DE OPERACIONES (TcolaOp) - Usada por PCB
// ==========================================================================
typedef struct {
    char id_dispositivo[MAXID];
    int duracion;
    int tipo_operacion;
} TELEMENTOCOP;

// Alias para compatibilidad con PCB.cpp
typedef TELEMENTOCOP RegistroE_S;

struct NodoColaOp {
    TELEMENTOCOP dato;
    struct NodoColaOp* sig;
};

typedef struct {
    NodoColaOp* frente;
    NodoColaOp* final;
} TcolaOp;

// --- Prototipos de funciones para TcolaOp ---
void iniciac(TcolaOp& c);
int vaciac(TcolaOp c);
void ponec(TcolaOp& c, TELEMENTOCOP dato);
void sacac(TcolaOp* c, TELEMENTOCOP* dato);


// ==========================================================================
// 4. LISTAS Y COLA DE PCBs (TcolaPcb) - Usada por dispositivos_IO
// ==========================================================================

// Define qué es un elemento de la cola de PCB (es un puntero a PCB)
typedef PCB* TELEMENTOCPCB;

// Nodo de la lista/cola de PCBs
struct NodoPCB {
    PCB* PCB;
    struct NodoPCB* sig;
};

// Definiciones de tipos de lista (ambas versiones para compatibilidad)
typedef NodoPCB* TlistaPCB;
typedef NodoPCB* TListaPCB;

// --- Estructura TcolaPcb (La que usa dispositivos_IO) ---
typedef struct {
    NodoPCB* frente;
    NodoPCB* final;
} TcolaPcb;

// --- Prototipos de funciones SOBRECARGADAS para TcolaPcb ---
void iniciac(TcolaPcb& c);
int vaciac(TcolaPcb c);

// Sobrecarga 1: Recibe referencia al objeto (para usar *pcb)
void ponec(TcolaPcb& c, PCB& dato);
// Sobrecarga 2: Recibe puntero directo (para usar pcb)
void ponec(TcolaPcb& c, PCB* dato);

// Saca un elemento (devuelve el puntero en 'dato')
void sacac(TcolaPcb* c, TELEMENTOCPCB* dato);


// ==========================================================================
// 5. OTRAS ESTRUCTURAS (Memoria y CPU)
// ==========================================================================

// --- Lista de Marcos ---
struct NodoMarco {
    marco* contenido;
    struct NodoMarco* sig;
};
// Alias dobles para compatibilidad con tu código existente
typedef NodoMarco* Tlistmarcos;
typedef NodoMarco* Tlistamarcos;

// --- Lista de Punteros a Memoria (Tabla de páginas del proceso) ---
struct NodoPunMem {
    marco* marco_asignado;
    struct NodoPunMem* sig;
};
typedef NodoPunMem* TlistPunMem;

// --- Registros de CPU ---
struct RegistroCPU {
    int AX, BX, CX, DX, PC, IR;
};
typedef RegistroCPU* TlistRegistros;

#endif // ESTRUCTURAS_H
