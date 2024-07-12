#ifndef ADMIN_H
#define ADMIN_H

#include <string>
#include "Biblioteca.h"

class Admin {
public:
    Admin(const std::string& nombre, const std::string& password, const std::string& passwordHash = "");

    std::string getNombre() const;
    bool verificarPassword(const std::string& password) const;
    std::string getPasswordHash() const;

    void agregarLibro(Biblioteca& biblioteca, const Libro& libro);
    void editarLibro(Biblioteca& biblioteca);
    void mostrarLibros(const Biblioteca& biblioteca) const;
    void eliminarLibro(Biblioteca& biblioteca, const std::string& titulo);
    void verEstadoPrestamos(const Biblioteca& biblioteca) const;
    void gestionarUsuarios(Biblioteca& biblioteca);
    void listarUsuarios(const Biblioteca& biblioteca) const;

private:
    std::string nombre;
    std::string passwordHash;
};

#endif // ADMIN_H
