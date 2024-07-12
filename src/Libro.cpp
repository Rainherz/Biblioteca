#include "Libro.h"
#include <algorithm>

using namespace std;

Libro::Libro(const string& titulo, const string& autor, const string& fechaPublicacion, const string& descripcion, const vector<string>& categorias)
    : titulo(titulo), autor(autor), fechaPublicacion(fechaPublicacion), descripcion(descripcion), categorias(categorias) {}

string Libro::getTitulo() const {
    return titulo;
}

string Libro::getAutor() const {
    return autor;
}

string Libro::getFechaPublicacion() const {
    return fechaPublicacion;
}

string Libro::getDescripcion() const {
    return descripcion;
}

vector<string> Libro::getCategorias() const {
    return categorias;
}

void Libro::setTitulo(const string& titulo) {
    this->titulo = titulo;
}

void Libro::setAutor(const string& autor) {
    this->autor = autor;
}

void Libro::setFechaPublicacion(const string& fechaPublicacion) {
    this->fechaPublicacion = fechaPublicacion;
}

void Libro::setDescripcion(const string& descripcion) {
    this->descripcion = descripcion;
}

void Libro::setCategorias(const vector<string>& categorias) {
    this->categorias = categorias;
}