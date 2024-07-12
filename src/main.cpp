#include <iostream>
#include <locale>
#include "Admin.h"
#include "Usuario.h"
#include "Biblioteca.h"
#include "Menu.h"
#include "GestorDatos.h"
#include "Utils.h"

using namespace std;

int main() {
    locale::global(locale(""));
    wcout.imbue(locale());
    Biblioteca biblioteca;
    GestorDatos::cargarLibros(biblioteca);
    GestorDatos::cargarCuentas(biblioteca);

    int opcionInicial;
    do {
        Menu::mostrarMenuPrincipal();
        opcionInicial = Menu::obtenerOpcion();
        switch (opcionInicial) {
            case 1: { // Iniciar sesión
                string nombre, password;
                cout << "Nombre de usuario: ";
                cin >> nombre;
                cout << "Contraseña: ";
                cin >> password;
                Admin* admin = biblioteca.obtenerAdmin(nombre);
                Usuario* usuario = biblioteca.obtenerUsuario(nombre);

                if (admin && admin->verificarPassword(hashPassword(password))) {
                    Menu::manejarMenuAdmin(admin, biblioteca);
                } else if (usuario && usuario->verificarPassword((password))) {
                    Menu::manejarMenuUsuario(usuario, biblioteca);
                } else {
                    cout << "Credenciales incorrectas." << endl;
                }
                break;
            }
            case 2: // Crear nueva cuenta
                GestorDatos::crearNuevaCuenta(biblioteca);
                break;
            case 3:
                cout << "Saliendo del programa...\n";
                break;
            default:
                Menu::mostrarMensajeError();
                break;
        }
    } while (opcionInicial != 3);

    GestorDatos::guardarLibros(biblioteca);
    GestorDatos::guardarCuentas(biblioteca);
    return 0;
}