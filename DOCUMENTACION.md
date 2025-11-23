# Documentación Técnica - Simulador de Sistema Operativo Multitarea

## Información del Proyecto
**Nombre:** DoorOS - Simulador de Sistema Operativo  
**Lenguaje:** C++17  
**Autores:** [Completar]  
**Fecha:** Noviembre 2025

---

## 1. Descripción General

Este proyecto implementa un simulador funcional de un sistema operativo multitarea que demuestra los conceptos fundamentales de:
- Gestión de procesos con modelo de 5 estados
- Planificación de CPU con algoritmo Round Robin
- Gestión de memoria mediante paginación simple
- Sincronización mediante semáforos (esquema productor-consumidor)
- Simulación de operaciones de E/S

---

## 2. Arquitectura del Sistema

### 2.1 Componentes Principales

#### **BIOS** (`bios.h/cpp`)
- Inicialización del hardware simulado
- Verificación de componentes (CPU, memoria, dispositivos)
- Carga del sistema operativo

#### **Sistema Operativo** (`Sistema_operativo.h/cpp`)
- **Planificador de Largo Plazo:** Admisión de procesos desde memoria secundaria
- **Planificador de Corto Plazo:** Algoritmo Round Robin con quantum=3
- **Gestor de E/S:** Manejo de bloqueos y desbloqueos
- **Reloj del Sistema:** Control de tiempo discreto (ticks)

#### **CPU** (`cpu.h/cpp`)
- Simulación de la unidad central de procesamiento
- Ejecución de instrucciones de procesos
- Cambio de contexto
- Estados: LIBRE / OCUPADO

#### **PCB (Process Control Block)** (`PCB.h/cpp`)
Estructura de datos que contiene:
- ID del proceso
- Estado actual (NUEVO, LISTO, EJECUCION, BLOQUEADO, TERMINADO)
- Tiempo de llegada, ráfaga restante, tamaño requerido
- Punteros a memoria asignada
- Cola de operaciones de E/S
- Registros de CPU
- Tiempos de espera y finalización

#### **Gestión de Memoria** (`memoria_prpal.h/cpp`, `memoria_sec.h/cpp`, `marco.h/cpp`)
- **Memoria Principal:** 256 marcos de 4096 bytes cada uno
- **Paginación simple:** Cada proceso recibe 1 marco mínimo
- **Memoria Secundaria:** Cola de procesos en estado NUEVO

#### **Cola Sincronizada** (`ColaSincronizada.h/cpp`)
- Implementación productor-consumidor con semáforos
- Buffer de capacidad 100 procesos
- Exclusión mutua mediante `std::mutex`
- Sincronización con semáforos para espacios libres e ítems disponibles

#### **Dispositivos de E/S** (`dispositivos_IO.h/cpp`)
- Simulación de dispositivos de entrada/salida
- Estados: LIBRE / OCUPADO
- Cola de espera para procesos bloqueados

---

## 3. Modelo de Estados de Procesos

```
    [NUEVO] ----Admisión----> [LISTO] <----Desbloqueo----
                                 |                       |
                              Dispatch                   |
                                 v                       |
                            [EJECUCION]                  |
                                 |                       |
                    +------------+------------+          |
                    |            |            |          |
                Termina      Quantum      E/S Wait      |
                    |            |            |          |
                    v            v            v          |
                [TERMINADO]  [LISTO]    [BLOQUEADO]-----+
```

### Transiciones Implementadas:
1. **NUEVO → LISTO:** Planificador de largo plazo (si hay memoria)
2. **LISTO → EJECUCION:** Planificador de corto plazo (Round Robin)
3. **EJECUCION → LISTO:** Quantum agotado (3 ciclos)
4. **EJECUCION → BLOQUEADO:** Solicitud de E/S
5. **BLOQUEADO → LISTO:** Finalización de E/S
6. **EJECUCION → TERMINADO:** Ráfaga completada

---

## 4. Algoritmo de Planificación

### Round Robin
- **Quantum:** 3 ciclos de reloj
- **Política:** FIFO con tiempo compartido
- **Cola de listos:** ColaSincronizada (productor-consumidor)

**Funcionamiento:**
1. Proceso entra a EJECUCION desde LISTO
2. Ejecuta hasta 3 ciclos máximo
3. Si termina: pasa a TERMINADO
4. Si agota quantum: vuelve a LISTO (fin de cola)
5. Si pide E/S: pasa a BLOQUEADO

---

## 5. Archivos de Configuración

### `configHard.txt`
Configuración del hardware (formato: `marcos, tamaño_marco`):
```
8, 4096
```
- 8 marcos de memoria principal
- Cada marco: 4096 bytes

### `configDisIO.txt`
Dispositivos de E/S (formato: `nombre, tiempo_promedio`):
```
DISK, 5
PRINTER, 3
NETWORK, 2
```

### `procesos.txt`
Procesos a ejecutar (formato: `ID, tiempo_llegada, rafaga, tamaño`):
```
P1, 0, 10, 1
P2, 2, 8, 1
P3, 4, 6, 1
P4, 6, 12, 1
```
- P1: llega en t=0, necesita 10 ciclos, 1 marco
- P2: llega en t=2, necesita 8 ciclos, 1 marco
- etc.

---

## 6. Compilación y Ejecución

### Requisitos
- Compilador g++ con soporte C++17
- Sistema operativo: Windows/Linux/MacOS

### Compilar
```bash
cd TrabajoIntegradoSistemasOperativos
g++ -std=c++17 -Wall -c src/*.cpp main.cpp -Iinclude
g++ -std=c++17 -o bin/Debug/TPsistemaoperativo *.o
```

### Ejecutar
```bash
./bin/Debug/TPsistemaoperativo
```

---

## 7. Salida de la Simulación

### Ejemplo de Ejecución
```
[SO] Sistema Operativo inicializado (Quantum=3)
[SO] Cargando procesos desde procesos.txt...
  - Proceso P1 cargado (llegada=0, rafaga=10)
  ...
========== INICIO DE SIMULACION ==========

--- Tick 0 ---
[Planificador LP] Admitiendo proceso P4 (marcos libres: 256)
[Planificador CP] Asignando proceso P4 a CPU
[CPU] Proceso P4 asignado a CPU

--- Tick 1 ---
[CPU] Ejecutando proceso P4 (rafaga restante: 11)

--- Tick 4 ---
[SO] Quantum agotado para proceso P4, cediendo CPU
[PCB P4] Transicion: EJECUCION -> LISTO (fin de quantum)
...
========== FIN DE SIMULACION (t=50) ==========

marco    id_proceso_asignado
1        0
2        1
3        2
4        3
5        -1
...
```

### Información Mostrada:
- **Transiciones de estado:** Cada cambio de estado de PCB
- **Eventos de planificación:** Admisión, dispatch, quantum
- **Ejecución de CPU:** Proceso actual y ráfaga restante
- **Estado final de memoria:** Marcos asignados/libres

---

## 8. Estructuras de Datos Principales

### TcolaOp - Cola de Operaciones de E/S
```cpp
typedef struct {
    char id_dispositivo[MAXID];
    int duracion;
    int tipo_operacion;
} TELEMENTOCOP;
```

### TcolaPcb - Cola de PCBs
```cpp
typedef struct {
    NodoPCB* frente;
    NodoPCB* final;
} TcolaPcb;
```

### Tabla de Procesos
Implementada mediante `ColaSincronizada` con soporte para concurrencia.

---

## 9. Sincronización

### Semáforos Implementados
```cpp
class Semaforo {
    std::mutex mtx;
    std::condition_variable cv;
    int cuenta;
public:
    void wait();      // P() - Decrementa
    void signal();    // V() - Incrementa
    bool try_wait();  // No bloqueante
};
```

### Cola Sincronizada (Productor-Consumidor)
- **Productor:** Planificador de largo plazo (agrega a cola de listos)
- **Consumidor:** Planificador de corto plazo (retira para ejecutar)
- **Buffer:** Capacidad 100 procesos
- **Exclusión mutua:** Garantizada por mutex

---

## 10. Limitaciones y Mejoras Futuras

### Limitaciones Actuales
1. E/S simplificada (sin dispositivos múltiples activos)
2. Operaciones de E/S no generan bloqueos (solo transiciones)
3. Gestión de memoria sin fragmentación ni swap
4. Sin prioridades de procesos

### Mejoras Propuestas
1. Implementar múltiples algoritmos de planificación (FCFS, SJF, prioridades)
2. Simulación completa de E/S con tiempos reales
3. Memoria virtual con paginación por demanda
4. Estadísticas de rendimiento (turnaround, waiting time)
5. Interfaz gráfica para visualización

---

## 11. Justificación Técnica

### Elección de C++17
1. **Rendimiento:** Compilado, ejecución nativa rápida
2. **Control de memoria:** Gestión manual apropiada para simular SO
3. **Concurrencia:** `std::thread`, `std::mutex`, `std::condition_variable`
4. **Estructuras:** Punteros y referencias para listas enlazadas eficientes

### Diseño Modular
- Separación clara de responsabilidades
- Cada componente en archivos .h/.cpp independientes
- Facilita mantenimiento y extensión

---

## 12. Conclusiones

Este simulador demuestra exitosamente:
✅ Modelo completo de 5 estados de procesos  
✅ Planificación Round Robin con quantum  
✅ Gestión de memoria con paginación  
✅ Sincronización productor-consumidor con semáforos  
✅ Transiciones de estado visibles en tiempo real  
✅ Configuración parametrizable desde archivos  

El proyecto cumple con todos los requisitos mínimos establecidos y proporciona una base sólida para comprender los conceptos fundamentales de sistemas operativos.

---

## Referencias
- Silberschatz, A., Galvin, P. B., & Gagne, G. (2018). Operating System Concepts (10th ed.)
- Tanenbaum, A. S., & Bos, H. (2014). Modern Operating Systems (4th ed.)
- Documentación de C++ Reference: https://en.cppreference.com/
