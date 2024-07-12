#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

#include <vector>
#include "Libro.h"
#include "Usuario.h"
#include "Admin.h"

class Admin;

struct Prestamo {
    Usuario usuario;
    Libro libro;
    std::string fechaPrestamo;
    std::string fechaDevolucion;
};

class Biblioteca {
private:
    std::vector<Libro> libros;
    std::vector<Usuario> usuarios;
    std::vector<Prestamo> prestamos;
    std::vector<Admin> admins;

public:
    void agregarUsuario(const Usuario& usuario);
    void eliminarUsuario(const std::string& nombreUsuario);
    Usuario* obtenerUsuario(const std::string& nombreUsuario);

    void agregarAdmin(const Admin& admin);
    void eliminarAdmin(const std::string& nombreUsuario);
    Admin* obtenerAdmin(const std::string& nombreUsuario);

    void agregarLibro(const Libro& libro);
    void eliminarLibro(const std::string& titulo);
    Libro* obtenerLibro(const std::string& titulo);
    std::vector<std::string> obtenerTitulosLibros() const;
    std::vector<std::string> obtenerTitulosLibrosDisponibles() const;

    std::vector<Libro> getLibros() const;
    std::vector<Usuario> getUsuarios() const;
    std::vector<Prestamo> getPrestamos() const;
    std::vector<Admin> getAdmins() const;

    void registrarPrestamo(const Usuario& usuario, const Libro& libro, const std::string& fechaPrestamo, const std::string& fechaDevolucion);
    void devolverPrestamo(const Usuario& usuario, const Libro& libro);
};

#endif // BIBLIOTECA_H
