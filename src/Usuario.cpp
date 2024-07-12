#include "Usuario.h"
#include "Biblioteca.h"
#include <iostream>
#include <algorithm>
#include <functional>
#include "Utils.h"
#include <set>
#include <cctype>
#include <locale>
#include <codecvt>
#include <fstream>
#include <sstream>

using namespace std;

Usuario::Usuario(const string& nombre, const string& password, const string& passwordHash)
    : nombre(nombre), passwordHash(passwordHash.empty() ? hashPassword(password) : passwordHash) {}


string Usuario::getPasswordHash() const {
    return passwordHash;
}

string Usuario::getNombre() const {
    return nombre;
}

bool Usuario::verificarPassword(const string& password) const {
    string inputHash = hashPassword(password);
    cout << "Input hash: " << inputHash << endl;
    cout << "Stored hash: " << passwordHash << endl;
    return passwordHash == inputHash;
}

void Usuario::solicitarPrestamo(Biblioteca& biblioteca, const string& tituloLibro, const string& fechaPrestamo, const string& fechaDevolucion) {
    vector<string> librosDisponibles = biblioteca.obtenerTitulosLibrosDisponibles();
    if (find(librosDisponibles.begin(), librosDisponibles.end(), tituloLibro) != librosDisponibles.end()) {
        string prestamo = nombre + "," + tituloLibro + "," + fechaPrestamo + "," + fechaDevolucion + ",F";
        vector<string> prestamos = cargarPrestamos();
        prestamos.push_back(prestamo);
        guardarPrestamos(prestamos);
        cout << "Préstamo registrado con éxito.\n";
    } else {
        cout << "El libro no está disponible para préstamo.\n";
    }
}

void Usuario::guardarPrestamos(const vector<string>& prestamos) {
    ofstream file("prestamos.txt");
    for (const auto& prestamo : prestamos) {
        file << prestamo << "\n";
    }
}

vector<string> Usuario::cargarPrestamos() {
    vector<string> prestamos;
    ifstream file("prestamos.txt");
    string line;
    while (getline(file, line)) {
        prestamos.push_back(line);
    }
    return prestamos;
}

void Usuario::devolverLibro(const string& tituloLibro) {
    vector<string> prestamos = cargarPrestamos();
    bool encontrado = false;
    for (auto& prestamo : prestamos) {
        istringstream iss(prestamo);
        string nombreUsuario, titulo, fechaPrestamo, fechaDevolucion, estado;
        getline(iss, nombreUsuario, ',');
        getline(iss, titulo, ',');
        getline(iss, fechaPrestamo, ',');
        getline(iss, fechaDevolucion, ',');
        getline(iss, estado);
        
        if (nombreUsuario == nombre && titulo == tituloLibro && estado == "F") {
            prestamo = nombreUsuario + "," + titulo + "," + fechaPrestamo + "," + fechaDevolucion + ",T";
            encontrado = true;
            break;
        }
    }
    
    if (encontrado) {
        guardarPrestamos(prestamos);
        cout << "Libro devuelto con éxito.\n";
    } else {
        cout << "No se encontró un préstamo activo para este libro.\n";
    }
}

void Usuario::listarLibros(const Biblioteca& biblioteca) const {
    const auto& libros = biblioteca.getLibros();
    
    if (libros.empty()) {
        cout << "No hay libros en la biblioteca.\n";
        return;
    }

    cout << "Lista de libros en la biblioteca:\n";
    cout << string(50, '-') << "\n";

    for (size_t i = 0; i < libros.size(); ++i) {
        const auto& libro = libros[i];
        cout << i + 1 << ". Título: " << libro.getTitulo() << "\n"
                  << "   Autor: " << libro.getAutor() << "\n"
                  << "   Fecha de publicación: " << libro.getFechaPublicacion() << "\n"
                  << "   Categoría: " << libro.getCategorias()[0] << "\n"
                  << "   Descripción: " << libro.getDescripcion() << "\n";
        
        if (i < libros.size() - 1) {
            cout << string(50, '-') << "\n";
        }
    }

    cout << string(50, '-') << "\n";
    cout << "Total de libros: " << libros.size() << "\n";
}

void Usuario::filtrarLibrosPorCategoria(const Biblioteca& biblioteca) const {
    set<string> todasCategorias;
    
    // Recopilar todas las categorías únicas
    for (const auto& libro : biblioteca.getLibros()) {
        if (!libro.getCategorias().empty()) {
            todasCategorias.insert(libro.getCategorias()[0]);
        }
    }

    while (true) {
        // Mostrar el menú de categorías
        cout << "Categorías disponibles:\n";
        int i = 1;
        vector<string> categoriasList(todasCategorias.begin(), todasCategorias.end());
        for (const auto& categoria : categoriasList) {
            cout << i << ". " << categoria << "\n";
            i++;
        }
        cout << "0. Salir\n";

        // Solicitar la selección del usuario
        int seleccion;
        cout << "Seleccione una categoría (0-" << todasCategorias.size() << "): ";
        cin >> seleccion;

        // Salir si el usuario selecciona 0
        if (seleccion == 0) {
            cout << "Saliendo del menú de categorías.\n";
            break;
        }

        // Validar la selección
        if (seleccion < 1 || seleccion > static_cast<int>(todasCategorias.size())) {
            cout << "Selección inválida. Por favor, intente de nuevo.\n";
            continue;
        }

        // Obtener la categoría seleccionada
        string categoriaSeleccionada = categoriasList[seleccion - 1];

        // Filtrar y mostrar los libros de la categoría seleccionada
        bool encontrados = false;
        cout << "\nLibros en la categoría '" << categoriaSeleccionada << "':\n\n";
        for (const auto& libro : biblioteca.getLibros()) {
            if (!libro.getCategorias().empty() && libro.getCategorias()[0] == categoriaSeleccionada) {
                cout << "Título: " << libro.getTitulo()
                          << "\nAutor: " << libro.getAutor()
                          << "\nFecha de publicación: " << libro.getFechaPublicacion()
                          << "\nDescripción: " << libro.getDescripcion()
                          << "\nCategoría: " << libro.getCategorias()[0]
                          << "\n\n";
                encontrados = true;
            }
        }

        if (!encontrados) {
            cout << "No se encontraron libros en la categoría '" << categoriaSeleccionada << "'.\n";
        }
    }
}   

vector<string> Usuario::obtenerLibrosPrestados() const {
    vector<string> librosPrestados;
    vector<string> prestamos = cargarPrestamos();
    
    for (const auto& prestamo : prestamos) {
        istringstream iss(prestamo);
        string nombreUsuario, titulo, fechaPrestamo, fechaDevolucion, estado;
        getline(iss, nombreUsuario, ',');
        getline(iss, titulo, ',');
        getline(iss, fechaPrestamo, ',');
        getline(iss, fechaDevolucion, ',');
        getline(iss, estado);
        
        if (nombreUsuario == nombre && estado == "F") {
            librosPrestados.push_back(titulo + " (Prestado el: " + fechaPrestamo + ", A devolver el: " + fechaDevolucion + ")");
        }
    }
    
    return librosPrestados;
}
