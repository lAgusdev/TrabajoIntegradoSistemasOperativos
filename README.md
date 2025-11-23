# Trabajo Integrado de Sistemas Operativos
Integrantes: Puente Villaroel Gaspar y Moar Agustín

# DoorOS - Simulador de Sistema Operativo Multitarea

## Descripción
Simulador funcional e interactivo de un sistema operativo que implementa:
- Gestión de procesos con modelo de 5 estados
- **Planificación FIFO (First In First Out)**
- Paginación simple de memoria
- Sincronización productor-consumidor con semáforos
- **Menú interactivo para crear procesos manualmente**
- **Temporización real con milisegundos (std::chrono)**
- Operaciones de E/S configurables

## Compilación

```bash
g++ -std=c++17 -Wall -c src/*.cpp main.cpp -Iinclude
g++ -std=c++17 -o bin/Debug/TPsistemaoperativo *.o
```

## Ejecución

```bash
./bin/Debug/TPsistemaoperativo
```

### Menú Interactivo
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

### Opción 1: Crear Proceso Manualmente
Permite definir interactivamente:
- ID del proceso
- Tiempo de llegada (ms)
- Ráfaga de CPU (ciclos)
- Tamaño en memoria (marcos)
- Operaciones de E/S (dispositivo, duración, tipo)

### Opción 2: Cargar desde Archivo
Carga múltiples procesos desde archivos como `procesos.txt`

### Opción 3: Iniciar Simulación
Ejecuta la simulación con **tiempos reales** (100ms por ciclo de CPU)

## Archivos de Configuración

- **configHard.txt**: Configuración de hardware (marcos de memoria)
- **configDisIO.txt**: Dispositivos de E/S disponibles
- **procesos.txt**: Procesos a ejecutar (ID, llegada, ráfaga, tamaño)

## Características Implementadas

✅ Modelo de 5 estados (NUEVO, LISTO, EJECUCION, BLOQUEADO, TERMINADO)  
✅ **Planificador FIFO (sin preempción)**  
✅ Planificador de largo plazo (admisión desde memoria secundaria)  
✅ Gestión de memoria con paginación (256 marcos)  
✅ Cola sincronizada con semáforos  
✅ **Menú interactivo completo**  
✅ **Creación manual de procesos con interfaz amigable**  
✅ **Temporización real con std::chrono (milisegundos)**  
✅ Visualización de transiciones de estado  
✅ Configuración parametrizable  

## Algoritmo de Planificación: FIFO

**First In First Out (FIFO)** también conocido como FCFS:
- El primer proceso que llega es el primero en ejecutarse
- No hay preempción: cada proceso ejecuta hasta terminar o pedir E/S
- Simple y justo en orden de llegada
- Sin overhead de cambio de contexto frecuente  

## Documentación Completa

Ver [DOCUMENTACION.md](DOCUMENTACION.md) para detalles técnicos completos.

## Estructura del Proyecto

```
├── include/          # Archivos de cabecera (.h)
├── src/              # Implementaciones (.cpp)
├── main.cpp          # Programa principal con menú interactivo
├── procesos.txt      # Archivo de procesos de prueba
├── configHard.txt    # Configuración de hardware
├── configDisIO.txt   # Configuración de dispositivos
└── bin/Debug/        # Ejecutables
```

## Autores
- Puente Villaroel Gaspar
- Moar Agustín

## Requisitos
- Compilador g++ con soporte C++17
- Sistema operativo: Windows/Linux/MacOS

## Novedades de la Versión Actual

🆕 **Menú Interactivo:** Interfaz de usuario completa  
🆕 **Creación Manual:** Define procesos paso a paso  
🆕 **FIFO:** Algoritmo de planificación simple y eficiente  
🆕 **Tiempos Reales:** Simulación con milisegundos usando `std::chrono`  
🆕 **Estado del Sistema:** Consulta el estado en cualquier momento

---

# Planificacion Original
El trabajo se realizara en el lenguaje C++ como se ve en el repositorio. El mismo se dividira en clases para cada elemento que requiere el Sistema Operativo. Dado al desconocimiento sobre el alcance del sistema operativo a presentar en el trabajo integrador, se determinaron dos planificaciones:

### Planificacion 1:
  - Buses en una clase sobrecargada (bus de datos, bus de direcciones, bus de ...)
  - Controlador de Interrupciones
  - Planificador de procesos
  - Procesador
  - Memoria RAM
  - Main (Dónde se ejuctaran el resto de clases)

### Planificacion 2:
  - Buses en una clase sobrecargada (bus de datos, bus de direcciones, bus de ...)
  - Controlador de Interrupciones
  - Planificador de procesos
  - Procesador
  - Memoria RAM
  - Main (Dónde se e
