#ifndef PROCESO_H
#define PROCESO_H
enum class EstadoProceso{
    NUEVO,
    LISTO,
    EJECUCION,
    BLOQUEADO,
    TERMINADO
};

typedef struct {
    int numero_marco;   // El número de marco físico (índice en MemoriaRAM).
    int bit_validez;    // 1: Página está en RAM (válida). 0: No está (inválida/en swap).
    int bit_referencia; // 1: Fue accedida recientemente. 0: No. (Para reemplazo de páginas).
} EntradaPagina;

typedef struct nodopag{
    EntradaPagina dato;
    struct nodopag*sig;
}Tnodopag;

typedef Tnodopag *Tpaginas;

typedef struct {
    int PC;        // Program Counter: Dirección de la próxima instrucción.
    int AX;        // Registro de Propósito General para contexto de datos.
    int SP;        // Stack Pointer: Puntero de Pila.
    int PSW;       // Program Status Word (Opcional): Para guardar flags de estado.
} Registros_CPU;

class Proceso{
        int id_proceso;
        EstadoProceso estado;
        float tiempo_cpu_requerido;
        float tiempo_restante;
        Registros_Cpu reg; //Contexto. Almacena el estado de la CPU cuando el proceso es desalojado o se bloquea. Permite reanudar la ejecución exactamente donde se detuvo.
        Tpaginas listpaginas;//La Tabla de Páginas del proceso, que mapea sus direcciones lógicas (páginas) a direcciones físicas (marcos).
        int evento;//Indica el tipo de recurso o evento específico por el que está esperando el proceso (ej. "Disco", "Impresora").
        int duracion_io_restante;//El número de ticks restantes que el proceso debe permanecer en estado BLOQUEADO hasta que su operación de E/S se complete.

    public:
        Proceso(int newid,float time_required);
        void solicitar_io(int duracion);
        void finalizar();
        void ObtenerInfo();
        virtual ~Proceso();
};
    Proceso::Proceso(int newid,float time_required){
        id_proceso=newid;
        tiempo_cpu_requerido=time_required;
        tiempo_restante=time_required;
        estado=EstadoProceso::NUEVO;
        listpaginas=NULL;
        duracion_io_restante=-1;
    }

    void Proceso::solicitar_io(int duracion){
        duracion_io_restante=duracion;
        estado=EstadoProceso::BLOQUEADO;

    }

    void Proceso::obtenerinfo(){

    }
    void Proceso::finalizar(){
        estado=EstadoProceso::TERMINADO;
    }
#endif // PROCESO_H
