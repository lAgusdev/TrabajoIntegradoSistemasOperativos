/* tdacolad.h  -  TDACola  implementacion dinamica*/

typedef int TELEMENTOC;
typedef struct nodo {
    Proceso *proceso;
    struct nodo *sig;
} NODOC;

typedef struct cola{
    NODOC *pri;
    NODOC *ult;
}TCOLA;

void iniciac(TCOLA *c);
void ponec (TCOLA *c, TELEMENTOC elem);
void sacac(TCOLA *c, TELEMENTOC *e);
void consultac(TCOLA cola, TELEMENTOC *e);
int vaciac(TCOLA cola);
int llenac(TCOLA cola);
