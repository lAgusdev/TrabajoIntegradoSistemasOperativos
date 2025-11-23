# Trabajo Integrado de Sistemas Operativos
Integrantes: Puente Villaroel Gaspar y Moar Agustín

# DoorOS - Simulador de Sistema Operativo Multitarea

## Descripción
Simulador funcional de un sistema operativo que implementa:
- Gestión de procesos con modelo de 5 estados
- Planificación Round Robin (quantum=3)
- Paginación simple de memoria
- Sincronización productor-consumidor con semáforos
- Operaciones de E/S con bloqueos

## Compilación

```bash
g++ -std=c++17 -Wall -c src/*.cpp main.cpp -Iinclude
g++ -std=c++17 -o bin/Debug/TPsistemaoperativo *.o
```

## Ejecución

```bash
./bin/Debug/TPsistemaoperativo
```

## Archivos de Configuración

- **configHard.txt**: Configuración de hardware (marcos de memoria)
- **configDisIO.txt**: Dispositivos de E/S disponibles
- **procesos.txt**: Procesos a ejecutar (ID, llegada, ráfaga, tamaño)

## Características Implementadas

✅ Modelo de 5 estados (NUEVO, LISTO, EJECUCION, BLOQUEADO, TERMINADO)  
✅ Planificador de corto plazo (Round Robin)  
✅ Planificador de largo plazo (admisión desde memoria secundaria)  
✅ Gestión de memoria con paginación (256 marcos)  
✅ Cola sincronizada con semáforos  
✅ Visualización de transiciones de estado  
✅ Configuración parametrizable  

## Documentación Completa

Ver [DOCUMENTACION.md](DOCUMENTACION.md) para detalles técnicos completos.

## Estructura del Proyecto

```
├── include/          # Archivos de cabecera (.h)
├── src/              # Implementaciones (.cpp)
├── main.cpp          # Programa principal
├── procesos.txt      # Archivo de procesos de prueba
├── configHard.txt    # Configuración de hardware
├── configDisIO.txt   # Configuración de dispositivos
└── bin/Debug/        # Ejecutables
```

## Requisitos
- Compilador g++ con soporte C++17
- Sistema operativo: Windows/Linux/MacOS

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
