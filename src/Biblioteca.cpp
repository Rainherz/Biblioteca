#include "Biblioteca.h"
#include "Libro.h"
#include "Usuario.h"
#include <algorithm>
#include <iostream>
#include <sstream>

using namespace std;

void Biblioteca::agregarUsuario(const Usuario& usuario) {
    usuarios.push_back(usuario);
}

void Biblioteca::eliminarUsuario(const string& nombreUsuario) {
    usuarios.erase(
        remove_if(usuarios.begin(), usuarios.end(), [&](const Usuario& usuario) {
            return usuario.getNombre() == nombreUsuario;
        }),
        usuarios.end()
    );
}

Usuario* Biblioteca::obtenerUsuario(const string& nombreUsuario) {
    auto it = find_if(usuarios.begin(), usuarios.end(), [&](const Usuario& usuario) {
        return usuario.getNombre() == nombreUsuario;
    });
    return it != usuarios.end() ? &(*it) : nullptr;
}

void Biblioteca::agregarLibro(const Libro& libro) {
    libros.push_back(libro);
}

void Biblioteca::eliminarLibro(const string& titulo) {
    libros.erase(
        remove_if(libros.begin(), libros.end(), [&](const Libro& libro) {
            return libro.getTitulo() == titulo;
        }),
        libros.end()
    );
}

vector<string> Biblioteca::obtenerTitulosLibros() const {
    vector<string> titulos;
    for (const auto& libro : libros) {
        titulos.push_back(libro.getTitulo());
    }
    return titulos;
}

vector<string> Biblioteca::obtenerTitulosLibrosDisponibles() const {
    vector<string> titulosDisponibles;
    vector<string> prestamos = Usuario::cargarPrestamos();
    
    for (const auto& libro : libros) {
        bool libroPrestado = false;
        for (const auto& prestamo : prestamos) {
            istringstream iss(prestamo);
            string nombreUsuario, titulo, fechaPrestamo, fechaDevolucion, estado;
            getline(iss, nombreUsuario, ',');
            getline(iss, titulo, ',');
            getline(iss, fechaPrestamo, ',');
            getline(iss, fechaDevolucion, ',');
            getline(iss, estado);
            
            if (titulo == libro.getTitulo() && estado == "F") {
                libroPrestado = true;
                break;
            }
        }
        
        if (!libroPrestado) {
            titulosDisponibles.push_back(libro.getTitulo());
        }
    }
    
    return titulosDisponibles;
}

void Biblioteca::agregarAdmin(const Admin& admin) {
    admins.push_back(admin);
}

void Biblioteca::eliminarAdmin(const string& nombreUsuario) {
    admins.erase(
        remove_if(admins.begin(), admins.end(), [&](const Admin& admin) {
            return admin.getNombre() == nombreUsuario;
        }),
        admins.end()
    );
}

Admin* Biblioteca::obtenerAdmin(const string& nombreUsuario) {
    auto it = find_if(admins.begin(), admins.end(), [&](const Admin& admin) {
        return admin.getNombre() == nombreUsuario;
    });
    return it != admins.end() ? &(*it) : nullptr;
}

Libro* Biblioteca::obtenerLibro(const string& titulo) {
    auto it = find_if(libros.begin(), libros.end(), [&](const Libro& libro) {
        return libro.getTitulo() == titulo;
    });
    return it != libros.end() ? &(*it) : nullptr;
}

vector<Admin> Biblioteca::getAdmins() const {
    return admins;
}

vector<Libro> Biblioteca::getLibros() const {
    return libros;
}

vector<Usuario> Biblioteca::getUsuarios() const {
    return usuarios;
}

vector<Prestamo> Biblioteca::getPrestamos() const {
    return prestamos;
}

void Biblioteca::registrarPrestamo(const Usuario& usuario, const Libro& libro, const string& fechaPrestamo, const string& fechaDevolucion) {
    Prestamo prestamo{usuario, libro, fechaPrestamo, fechaDevolucion};
    prestamos.push_back(prestamo);
}

void Biblioteca::devolverPrestamo(const Usuario& usuario, const Libro& libro) {
    prestamos.erase(
        remove_if(prestamos.begin(), prestamos.end(), [&](const Prestamo& prestamo) {
            return prestamo.usuario.getNombre() == usuario.getNombre() && prestamo.libro.getTitulo() == libro.getTitulo();
        }),
        prestamos.end()
    );
}