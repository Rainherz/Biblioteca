#ifndef LIBRO_H
#define LIBRO_H

#include <string>
#include <vector>

class Libro {
public:
    Libro(const std::string& titulo, const std::string& autor, const std::string& fechaPublicacion, const std::string& descripcion, const std::vector<std::string>& categorias);

    std::string getTitulo() const;
    std::string getAutor() const;
    std::string getFechaPublicacion() const;
    std::string getDescripcion() const;
    std::vector<std::string> getCategorias() const;

    void setTitulo(const std::string& titulo);
    void setAutor(const std::string& autor);
    void setFechaPublicacion(const std::string& fechaPublicacion);
    void setDescripcion(const std::string& descripcion);
    void setCategorias(const std::vector<std::string>& categorias);
    
private:
    std::string titulo;
    std::string autor;
    std::string fechaPublicacion;
    std::string descripcion;
    std::vector<std::string> categorias;
};

#endif // LIBRO_H