#ifndef MENU_H
#define MENU_H

#include <iostream>
#include "Admin.h"
#include "Biblioteca.h"
#include "Usuario.h"
#include "Libro.h"
#include <sstream>
#include <limits>
using namespace std;

class Menu
{

public:
    static void mostrarMenuPrincipal()
    {
        cout << "Menú principal\n";
        cout << "1. Iniciar sesión\n";
        cout << "2. Registrarse\n";
        cout << "3. Salir\n";
    }

    static void mostrarMenuAdmin()
    {
        cout << "Menú de administrador\n";
        cout << "1. Agregar libro\n";
        cout << "2. Editar libro\n";
        cout << "3. Eliminar libro\n";
        cout << "4. Ver préstamos\n";
        cout << "5. Ver usuarios\n";
        cout << "6. Ver libros\n";
        cout << "0. Salir\n";
    }

    static void mostrarMenuUsuario()
    {
        cout << "Menú de usuario\n";
        cout << "1. Prestar libro\n";
        cout << "2. Devolver libro\n";
        cout << "3. Ver libros\n";
        cout << "4. Filtrar libros por categoría\n";
        cout << "0. Salir\n";
    }

    static int obtenerOpcion() {
    int opcion;
    while (true) {
        cout << "Seleccione una opción: ";
        if (cin >> opcion) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return opcion;
        } else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Error: Entrada inválida. Por favor, ingrese un número." << endl;
        }
    }
}

    static void mostrarMensajeError()
    {
        cout << "Error: selección inválida.\n";
    }

    static void manejarMenuAdmin(Admin *admin, Biblioteca &biblioteca)
    {
        int opcion;
        do
        {
            mostrarMenuAdmin();
            opcion = obtenerOpcion();
            switch (opcion)
            {
            case 1:
                agregarLibro(admin, biblioteca);
                break;
            case 2:
                admin->editarLibro(biblioteca);
                break;
            case 3:
                eliminarLibro(admin, biblioteca);
                break;
            case 4:
                admin->verEstadoPrestamos(biblioteca);
                break;
            case 5:
                admin->listarUsuarios(biblioteca);
                break;
            case 6:
                admin->mostrarLibros(biblioteca);
                break;
            case 0:
                cout << "Saliendo...\n";
                break;
            default:
                cout << "Opcion no valida.\n";
                break;
            }
        } while (opcion != 0);
    }

    static void manejarMenuUsuario(Usuario *usuario, Biblioteca &biblioteca)
    {
        int opcion;
        do
        {
            mostrarMenuUsuario();
            opcion = obtenerOpcion();
            switch (opcion)
            {
            case 1:
                solicitarPrestamo(usuario, biblioteca);
                break;
            case 2:
                devolverLibro(usuario);
                break;
            case 3:
                usuario->listarLibros(biblioteca);
                break;
            case 4:
                usuario->filtrarLibrosPorCategoria(biblioteca);
                break;
            case 0:
                cout << "Saliendo...\n";
                break;
            default:
                cout << "Opcion no valida.\n";
                break;
            }
        } while (opcion != 0);
    }

private:
    static void agregarLibro(Admin *admin, Biblioteca &biblioteca)
    {
        string titulo, autor, fechaPublicacion, descripcion, categoria;
        vector<string> categorias;
        cout << "Titulo: ";
        getline(cin, titulo);
        cout << "Autor: ";
        getline(cin, autor);
        cout << "Fecha de publicacion: ";
        getline(cin, fechaPublicacion);
        cout << "Descripcion: ";
        getline(cin, descripcion);
        cout << "Categorias (separadas por ';'): ";
        getline(cin, categoria);

        istringstream categoriasStream(categoria);
        string cat;
        while (getline(categoriasStream, cat, ';'))
        {
            categorias.push_back(cat);
        }
        admin->agregarLibro(biblioteca, Libro(titulo, autor, fechaPublicacion, descripcion, categorias));
    }

    static void eliminarLibro(Admin *admin, Biblioteca &biblioteca)
    {
        string titulo;
        cout << "Titulo del libro a eliminar: ";
        getline(cin, titulo);
        admin->eliminarLibro(biblioteca, titulo);
    }

    static void solicitarPrestamo(Usuario *usuario, Biblioteca &biblioteca)
    {
        vector<string> titulosLibrosDisponibles = biblioteca.obtenerTitulosLibrosDisponibles();
        if (titulosLibrosDisponibles.empty())
        {
            cout << "No hay libros disponibles para préstamo en este momento.\n";
        }
        else
        {
            cout << "Libros disponibles para préstamo:\n";
            for (size_t i = 0; i < titulosLibrosDisponibles.size(); ++i)
            {
                cout << i + 1 << ". " << titulosLibrosDisponibles[i] << "\n";
            }

            cout << "\nIngrese el número del libro que desea pedir prestado (0 para cancelar): ";
            int seleccion;
            cin >> seleccion;
            cin.ignore();

            if (seleccion > 0 && seleccion <= static_cast<int>(titulosLibrosDisponibles.size()))
            {
                string titulo = titulosLibrosDisponibles[seleccion - 1];
                string fechaPrestamo, fechaDevolucion;

                cout << "Fecha de prestamo (YYYY-MM-DD): ";
                getline(cin, fechaPrestamo);
                cout << "Fecha de devolucion (YYYY-MM-DD): ";
                getline(cin, fechaDevolucion);

                usuario->solicitarPrestamo(biblioteca, titulo, fechaPrestamo, fechaDevolucion);
            }
            else if (seleccion != 0)
            {
                cout << "Selección inválida.\n";
            }
        }
    }

    static void devolverLibro(Usuario *usuario)
    {
        vector<string> librosPrestados = usuario->obtenerLibrosPrestados();
        if (librosPrestados.empty())
        {
            cout << "No tienes libros prestados actualmente.\n";
        }
        else
        {
            cout << "Tus libros prestados:\n";
            for (size_t i = 0; i < librosPrestados.size(); ++i)
            {
                cout << i + 1 << ". " << librosPrestados[i] << "\n";
            }
            cout << "\nIngrese el número del libro que desea devolver (0 para cancelar): ";
            int seleccion;
            cin >> seleccion;
            cin.ignore();

            if (seleccion > 0 && seleccion <= static_cast<int>(librosPrestados.size()))
            {
                string tituloLibro = librosPrestados[seleccion - 1];
                size_t pos = tituloLibro.find(" (Prestado");
                if (pos != string::npos)
                {
                    tituloLibro = tituloLibro.substr(0, pos);
                }
                usuario->devolverLibro(tituloLibro);
            }
            else if (seleccion != 0)
            {
                cout << "Selección inválida.\n";
            }
        }
    }
};

#endif // MENU_H