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

### FIFO (First In First Out)
- **Política:** El primer proceso que llega es el primero en ser atendido
- **No hay preempción:** El proceso ejecuta hasta terminar o solicitar E/S
- **Cola de listos:** ColaSincronizada (productor-consumidor)

**Funcionamiento:**
1. Proceso entra a EJECUCION desde LISTO (orden de llegada)
2. Ejecuta hasta completar su ráfaga (sin interrupciones)
3. Si termina: pasa a TERMINADO
4. Si pide E/S: pasa a BLOQUEADO
5. Cuando finaliza E/S: vuelve a LISTO (al final de la cola)

**Ventajas:**
- Simple de implementar
- Sin overhead de cambio de contexto frecuente
- Justo en orden de llegada

**Desventajas:**
- Tiempos de espera pueden ser largos
- Efecto convoy (procesos cortos esperan a largos)

---

## 5. Interfaz de Usuario

### Menú Principal Interactivo
```
=========================================
     DOOROS - Sistema Operativo
   Algoritmo: FIFO (First In First Out)
=========================================
1. Crear proceso manualmente
2. Cargar procesos desde archivo
3. Iniciar simulacion
4. Ver estado del sistema
5. Salir
=========================================
```

### Creación Manual de Procesos
El usuario puede crear procesos interactivamente especificando:
- **ID del proceso:** Identificador único
- **Tiempo de llegada:** En milisegundos
- **Ráfaga de CPU:** Número de ciclos necesarios
- **Tamaño requerido:** Marcos de memoria
- **Operaciones de E/S:** Por cada operación:
  - Dispositivo (DISK, PRINTER, NETWORK)
  - Duración (ciclos)
  - Tipo (lectura/escritura)

### Temporizador Real
- Cada ciclo de CPU simula **100ms** de tiempo real
- Usa `std::chrono` para delays precisos
- Los tiempos se muestran en ms y ticks

---

## 6. Archivos de Configuración

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

## 7. Compilación y Ejecución

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

**Modo interactivo:**
1. El programa inicia con el menú principal
2. Opción 1: Crear procesos uno por uno con la interfaz
3. Opción 2: Cargar múltiples procesos desde archivo
4. Opción 4: Ver estado actual del sistema
5. Opción 3: Iniciar la simulación (se ejecuta con tiempos reales)
6. Opción 5: Salir del programa

---

## 8. Salida de la Simulación

### Ejemplo de Ejecución (FIFO)
```
[SO] Sistema Operativo inicializado (Algoritmo: FIFO)
[SO] Tiempo por ciclo de CPU: 100ms
[SO] Cargando procesos desde procesos.txt...
  - Proceso P1 cargado (llegada=0, rafaga=10)
  ...
========== INICIO DE SIMULACION (FIFO) ==========

--- Tick 0 (0ms) ---
[Planificador LP] Admitiendo proceso P1 (marcos libres: 256)
[Planificador CP - FIFO] Asignando proceso P1 a CPU
[CPU] Proceso P1 asignado a CPU

--- Tick 1 (100ms) ---
[CPU] Ejecutando proceso P1 (rafaga restante: 9)

--- Tick 10 (1000ms) ---
[SO] Proceso P1 ha terminado
[PCB P1] Transicion: EJECUCION -> TERMINADO (t=10)
[Planificador CP - FIFO] Asignando proceso P2 a CPU
...
========== FIN DE SIMULACION (t=50) ==========
```

**Nota:** Con FIFO, cada proceso ejecuta completamente sin interrupciones
(no hay cambios de contexto por quantum como en Round Robin)

### Información Mostrada:
- **Transiciones de estado:** Cada cambio de estado de PCB
- **Eventos de planificación:** Admisión, dispatch, quantum
- **Ejecución de CPU:** Proceso actual y ráfaga restante
- **Estado final de memoria:** Marcos asignados/libres

---

## 9. Estructuras de Datos Principales

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

## 10. Sincronización

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

## 11. Limitaciones y Mejoras Futuras

### Limitaciones Actuales
1. E/S simplificada (sin dispositivos múltiples activos)
2. Operaciones de E/S no generan bloqueos (solo transiciones)
3. Gestión de memoria sin fragmentación ni swap
4. Sin prioridades de procesos

### Mejoras Propuestas
1. Implementar múltiples algoritmos de planificación seleccionables (FIFO, Round Robin, SJF, prioridades)
2. Simulación completa de E/S con tiempos reales y bloqueos efectivos
3. Memoria virtual con paginación por demanda y algoritmos de reemplazo
4. Estadísticas de rendimiento (turnaround time, waiting time, throughput)
5. Interfaz gráfica para visualización en tiempo real

---

## 12. Justificación Técnica

### Elección de C++17
1. **Rendimiento:** Compilado, ejecución nativa rápida
2. **Control de memoria:** Gestión manual apropiada para simular SO
3. **Concurrencia:** `std::thread`, `std::mutex`, `std::condition_variable`
4. **Estructuras:** Punteros y referencias para listas enlazadas eficientes

### Diseño Modular
- Separación clara de responsabilidades
- Cada componente en archivos .h/.cpp independientes
- Facilita mantenimiento y extensión

### Interfaz Interactiva
- Menú intuitivo para operación del simulador
- Creación manual de procesos con validación
- Tiempos reales con `std::chrono` para mayor realismo

---

## 13. Conclusiones

Este simulador demuestra exitosamente:
✅ Modelo completo de 5 estados de procesos  
✅ Planificación FIFO (First In First Out)  
✅ Gestión de memoria con paginación simple
✅ Sincronización productor-consumidor con semáforos  
✅ Transiciones de estado visibles en tiempo real  
✅ **Menú interactivo para operación del sistema**  
✅ **Creación manual de procesos con interfaz**  
✅ **Temporización real con milisegundos (std::chrono)**  
✅ Configuración parametrizable desde archivos  

El proyecto cumple con todos los requisitos establecidos y proporciona una interfaz amigable para experimentar con conceptos de sistemas operativos.

---

## Referencias
- Silberschatz, A., Galvin, P. B., & Gagne, G. (2018). Operating System Concepts (10th ed.)
- Tanenbaum, A. S., & Bos, H. (2014). Modern Operating Systems (4th ed.)
- Documentación de C++ Reference: https://en.cppreference.com/
