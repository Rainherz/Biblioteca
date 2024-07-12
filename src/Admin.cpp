#include "Admin.h"
#include "Utils.h"
#include <iostream>
#include <algorithm>
#include <functional>
#include <sstream>

using namespace std;

Admin::Admin(const string& nombre, const string& password, const string& passwordHash)
    : nombre(nombre), passwordHash(passwordHash.empty() ? hashPassword(password) : passwordHash) {}

string Admin::getNombre() const {
    return nombre;
}

bool Admin::verificarPassword(const string& password) const {
    return passwordHash == hashPassword(password);
}

string Admin::getPasswordHash() const {
    return passwordHash;
}

void Admin::agregarLibro(Biblioteca& biblioteca, const Libro& libro) {
    biblioteca.agregarLibro(libro);
    cout << "Libro agregado: " << libro.getTitulo() << endl;
}

void Admin::editarLibro(Biblioteca& biblioteca) {
    vector<string> titulos = biblioteca.obtenerTitulosLibros();
    
    if (titulos.empty()) {
        cout << "No hay libros en la biblioteca para editar." << endl;
        return;
    }

    cout << "Libros disponibles para editar:" << endl;
    for (size_t i = 0; i < titulos.size(); ++i) {
        cout << i + 1 << ". " << titulos[i] << endl;
    }

    int seleccion;
    cout << "Seleccione el número del libro que desea editar (0 para cancelar): ";
    cin >> seleccion;
    cin.ignore();

    if (seleccion <= 0 || seleccion > static_cast<int>(titulos.size())) {
        cout << "Selección inválida o cancelada." << endl;
        return;
    }

    string tituloSeleccionado = titulos[seleccion - 1];
    Libro* libroExistente = biblioteca.obtenerLibro(tituloSeleccionado);

    if (libroExistente) {
        string nuevoTitulo, nuevoAutor, nuevaFechaPublicacion, nuevaDescripcion, nuevaCategoria;

        cout << "Presione Enter para mantener el valor actual, o ingrese un nuevo valor." << endl;

        cout << "Título actual: " << libroExistente->getTitulo() << endl;
        cout << "Nuevo título: ";
        getline(cin, nuevoTitulo);
        if (!nuevoTitulo.empty()) libroExistente->setTitulo(nuevoTitulo);

        cout << "Autor actual: " << libroExistente->getAutor() << endl;
        cout << "Nuevo autor: ";
        getline(cin, nuevoAutor);
        if (!nuevoAutor.empty()) libroExistente->setAutor(nuevoAutor);

        cout << "Fecha de publicación actual: " << libroExistente->getFechaPublicacion() << endl;
        cout << "Nueva fecha de publicación: ";
        getline(cin, nuevaFechaPublicacion);
        if (!nuevaFechaPublicacion.empty()) libroExistente->setFechaPublicacion(nuevaFechaPublicacion);

        cout << "Descripción actual: " << libroExistente->getDescripcion() << endl;
        cout << "Nueva descripción: ";
        getline(cin, nuevaDescripcion);
        if (!nuevaDescripcion.empty()) libroExistente->setDescripcion(nuevaDescripcion);

        cout << "Categoría actual: " << libroExistente->getCategorias()[0] << endl;
        cout << "Nueva categoría: ";
        getline(cin, nuevaCategoria);
        if (!nuevaCategoria.empty()) libroExistente->setCategorias({nuevaCategoria});

        cout << "Libro editado: " << libroExistente->getTitulo() << endl;
    } else {
        cout << "Error: No se pudo encontrar el libro seleccionado." << endl;
    }
}

void Admin::eliminarLibro(Biblioteca& biblioteca, const string& titulo) {
    biblioteca.eliminarLibro(titulo);
    cout << "Libro eliminado: " << titulo << endl;
}

void Admin::mostrarLibros(const Biblioteca& biblioteca) const {
    vector<string> titulos = biblioteca.obtenerTitulosLibros();
    
    if (titulos.empty()) {
        cout << "No hay libros en la biblioteca.\n";
        return;
    }

    cout << "Lista de libros en la biblioteca:\n";
    for (size_t i = 0; i < titulos.size(); ++i) {
        cout << i + 1 << ". " << titulos[i] << "\n";
    }
    cout << "Total de libros: " << titulos.size() << "\n";
}

void Admin::verEstadoPrestamos(const Biblioteca& biblioteca) const {
    vector<string> prestamos = Usuario::cargarPrestamos();
    
    if (prestamos.empty()) {
        cout << "No hay préstamos registrados." << endl;
        return;
    }

    for (const auto& prestamo : prestamos) {
        istringstream iss(prestamo);
        string nombreUsuario, tituloLibro, fechaPrestamo, fechaDevolucion, estado;
        
        getline(iss, nombreUsuario, ',');
        getline(iss, tituloLibro, ',');
        getline(iss, fechaPrestamo, ',');
        getline(iss, fechaDevolucion, ',');
        getline(iss, estado);

        cout << "Usuario: " << nombreUsuario
             << ", Libro: " << tituloLibro
             << ", Fecha de Préstamo: " << fechaPrestamo
             << ", Fecha de Devolución: " << fechaDevolucion
             << ", Estado: " << (estado == "F" ? "Prestado" : "Devuelto") << endl;
    }
}

void Admin::listarUsuarios(const Biblioteca& biblioteca) const {
    for (const auto& usuario : biblioteca.getUsuarios()) {
        cout << "Usuario: " << usuario.getNombre() << endl;
    }
}