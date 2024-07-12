cmake_minimum_required(VERSION 3.10)
project(PS_Proyecto)

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
add_executable(PS_Proyecto ${SOURCES})

# Encuentra e incluye las bibliotecas necesarias
find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)

# Enlaza las bibliotecas
target_link_libraries(PS_Proyecto CURL::libcurl nlohmann_json::nlohmann_json)
