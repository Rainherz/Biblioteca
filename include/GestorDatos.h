#ifndef GESTORDATOS_H
#define GESTORDATOS_H

#include "Biblioteca.h"
#include <string>

class GestorDatos {
public:
    static void cargarLibros(Biblioteca& biblioteca);
    static void guardarLibros(const Biblioteca& biblioteca);
    static void cargarCuentas(Biblioteca& biblioteca);
    static void guardarCuentas(const Biblioteca& biblioteca);
    static void crearNuevaCuenta(Biblioteca& biblioteca);
};

#endif // GESTORDATOS_H