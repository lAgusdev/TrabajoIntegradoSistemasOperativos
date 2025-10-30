#include "tdacolad.h"
#include <stdlib.h>

void iniciac(TCOLA *c) {
    c->pri = c->ult = NULL;
}

int vaciac(TCOLA cola){
    return cola.pri == NULL;
}

void ponec(TCOLA *c, TELEMENTOC elem){
    NODOC *pnuevo;
    pnuevo = (NODOC*)malloc(sizeof(NODOC));
    pnuevo->proceso = elem;
    pnuevo->sig = NULL;
    if(c->pri == NULL)
        c->pri = pnuevo;
    else
        c->ult->sig = pnuevo;
    c->ult = pnuevo;
}

void sacac(TCOLA *c, TELEMENTOC *e){
    NODOC *paux;
    if(c->pri != NULL){
        *e = c->pri->proceso;
        paux = c->pri;
        c->pri = c->pri->sig;
        if(c->pri == NULL)
           c->ult = NULL;
        free(paux);
    }
}

void consultac(TCOLA cola, TELEMENTOC *e){
    if(cola.pri != NULL)
        *e = cola.pri->proceso;
}

//Considerando el tamanio del heap como "inifinito" una cola dinamica nunca podria estar llena
int llenac(TCOLA cola){
    return 0;
}
