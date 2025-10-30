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
    int numero_marco;   // El n�mero de marco f�sico (�ndice en MemoriaRAM).
    int bit_validez;    // 1: P�gina est� en RAM (v�lida). 0: No est� (inv�lida/en swap).
    int bit_referencia; // 1: Fue accedida recientemente. 0: No. (Para reemplazo de p�ginas).
} EntradaPagina;

typedef struct nodopag{
    EntradaPagina dato;
    struct nodopag*sig;
}Tnodopag;

typedef Tnodopag *Tpaginas;

typedef struct {
    int PC;        // Program Counter: Direcci�n de la pr�xima instrucci�n.
    int AX;        // Registro de Prop�sito General para contexto de datos.
    int SP;        // Stack Pointer: Puntero de Pila.
    int PSW;       // Program Status Word (Opcional): Para guardar flags de estado.
} Registros_CPU;

class Proceso{
        int id_proceso;
        EstadoProceso estado;
        float tiempo_cpu_requerido;
        float tiempo_restante;
        Registros_Cpu reg; //Contexto. Almacena el estado de la CPU cuando el proceso es desalojado o se bloquea. Permite reanudar la ejecuci�n exactamente donde se detuvo.
        Tpaginas listpaginas;//La Tabla de P�ginas del proceso, que mapea sus direcciones l�gicas (p�ginas) a direcciones f�sicas (marcos).
        int evento;//Indica el tipo de recurso o evento espec�fico por el que est� esperando el proceso (ej. "Disco", "Impresora").
        int duracion_io_restante;//El n�mero de ticks restantes que el proceso debe permanecer en estado BLOQUEADO hasta que su operaci�n de E/S se complete.

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
