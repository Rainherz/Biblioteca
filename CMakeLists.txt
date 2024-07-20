cmake_minimum_required(VERSION 3.10)
project(Biblioteca)

set(CMAKE_CXX_STANDARD 17)

# Incluye los directorios de los archivos de encabezado
include_directories(include)

# Lista de archivos fuente
set(SOURCES 
    src/Admin.cpp
    src/Biblioteca.cpp
    src/Libro.cpp
    src/main.cpp
    src/Usuario.cpp
    src/GestorDatos.cpp
)

# Añade el ejecutable y los archivos fuente
add_executable(Biblioteca ${SOURCES})