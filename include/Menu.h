#ifndef MENU_H
#define MENU_H

#include <iostream>
using namespace std;
#include "Admin.h"
#include "Biblioteca.h"
#include "Usuario.h"
#include "Libro.h"
#include <sstream>

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

    static int obtenerOpcion()
    {
        int opcion;
        cout << "Seleccione una opción: ";
        cin >> opcion;
        cin.ignore();
        return opcion;
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
        std::string titulo, autor, fechaPublicacion, descripcion, categoria;
        std::vector<std::string> categorias;
        std::cout << "Titulo: ";
        std::getline(std::cin, titulo);
        std::cout << "Autor: ";
        std::getline(std::cin, autor);
        std::cout << "Fecha de publicacion: ";
        std::getline(std::cin, fechaPublicacion);
        std::cout << "Descripcion: ";
        std::getline(std::cin, descripcion);
        std::cout << "Categorias (separadas por ';'): ";
        std::getline(std::cin, categoria);

        std::istringstream categoriasStream(categoria);
        std::string cat;
        while (std::getline(categoriasStream, cat, ';'))
        {
            categorias.push_back(cat);
        }
        admin->agregarLibro(biblioteca, Libro(titulo, autor, fechaPublicacion, descripcion, categorias));
    }

    static void eliminarLibro(Admin *admin, Biblioteca &biblioteca)
    {
        std::string titulo;
        std::cout << "Titulo del libro a eliminar: ";
        std::getline(std::cin, titulo);
        admin->eliminarLibro(biblioteca, titulo);
    }

    static void solicitarPrestamo(Usuario *usuario, Biblioteca &biblioteca)
    {
        std::vector<std::string> titulosLibrosDisponibles = biblioteca.obtenerTitulosLibrosDisponibles();
        if (titulosLibrosDisponibles.empty())
        {
            std::cout << "No hay libros disponibles para préstamo en este momento.\n";
        }
        else
        {
            std::cout << "Libros disponibles para préstamo:\n";
            for (size_t i = 0; i < titulosLibrosDisponibles.size(); ++i)
            {
                std::cout << i + 1 << ". " << titulosLibrosDisponibles[i] << "\n";
            }

            std::cout << "\nIngrese el número del libro que desea pedir prestado (0 para cancelar): ";
            int seleccion;
            std::cin >> seleccion;
            std::cin.ignore();

            if (seleccion > 0 && seleccion <= static_cast<int>(titulosLibrosDisponibles.size()))
            {
                std::string titulo = titulosLibrosDisponibles[seleccion - 1];
                std::string fechaPrestamo, fechaDevolucion;

                std::cout << "Fecha de prestamo (YYYY-MM-DD): ";
                std::getline(std::cin, fechaPrestamo);
                std::cout << "Fecha de devolucion (YYYY-MM-DD): ";
                std::getline(std::cin, fechaDevolucion);

                usuario->solicitarPrestamo(biblioteca, titulo, fechaPrestamo, fechaDevolucion);
            }
            else if (seleccion != 0)
            {
                std::cout << "Selección inválida.\n";
            }
        }
    }

    static void devolverLibro(Usuario *usuario)
    {
        std::vector<std::string> librosPrestados = usuario->obtenerLibrosPrestados();
        if (librosPrestados.empty())
        {
            std::cout << "No tienes libros prestados actualmente.\n";
        }
        else
        {
            std::cout << "Tus libros prestados:\n";
            for (size_t i = 0; i < librosPrestados.size(); ++i)
            {
                std::cout << i + 1 << ". " << librosPrestados[i] << "\n";
            }
            std::cout << "\nIngrese el número del libro que desea devolver (0 para cancelar): ";
            int seleccion;
            std::cin >> seleccion;
            std::cin.ignore();

            if (seleccion > 0 && seleccion <= static_cast<int>(librosPrestados.size()))
            {
                std::string tituloLibro = librosPrestados[seleccion - 1];
                size_t pos = tituloLibro.find(" (Prestado");
                if (pos != std::string::npos)
                {
                    tituloLibro = tituloLibro.substr(0, pos);
                }
                usuario->devolverLibro(tituloLibro);
            }
            else if (seleccion != 0)
            {
                std::cout << "Selección inválida.\n";
            }
        }
    }
};

#endif // MENU_H