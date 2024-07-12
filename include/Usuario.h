// File: include/Usuario.h
#ifndef USUARIO_H
#define USUARIO_H

#include <string>
#include <vector>
#include "Libro.h"

class Biblioteca;

class Usuario {
public:
    Usuario(const std::string& nombre, const std::string& password, const std::string& passwordHash = "");

    std::string getNombre() const;
    bool verificarPassword(const std::string& password) const;
    std::string getPasswordHash() const;

    void solicitarPrestamo(Biblioteca& biblioteca, const std::string& tituloLibro, const std::string& fechaPrestamo, const std::string& fechaDevolucion);
    void devolverLibro(const std::string& tituloLibro);
    void listarLibros(const Biblioteca& biblioteca) const;
    void filtrarLibrosPorCategoria(const Biblioteca& biblioteca) const;

    static void guardarPrestamos(const std::vector<std::string>& prestamos);
    static std::vector<std::string> cargarPrestamos();
    std::vector<std::string> obtenerLibrosPrestados() const;


private:
    std::string nombre;
    std::string passwordHash;
    std::vector<Libro> librosPrestados;
};

#endif // USUARIO_H
