#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Admin.h"
#include "Usuario.h"
#include "Biblioteca.h"
#include "Libro.h"
#include "Utils.h"
#include <codecvt>
#include <locale>

using namespace std;

void cargarLibros(Biblioteca& biblioteca) {
    wifstream file("libros.txt");
    file.imbue(locale(locale(), new codecvt_utf8<wchar_t>));
    
    if (!file.is_open()) {
        cerr << "Error: No se pudo abrir el archivo libros.txt" << endl;
        return;
    }

    wstring line;
    while (getline(file, line)) {
        wstringstream ss(line);
        wstring titulo, autor, fechaPublicacion, descripcion, categorias;

        if (getline(ss, titulo, L',') &&
            getline(ss, autor, L',') &&
            getline(ss, fechaPublicacion, L',') &&
            getline(ss, descripcion, L',') &&
            getline(ss, categorias)) {

            vector<string> categoriasList;
            wstringstream categoriasStream(categorias);
            wstring categoria;
            while (getline(categoriasStream, categoria, L';')) {
                categoriasList.push_back(wstring_convert<codecvt_utf8<wchar_t>>().to_bytes(categoria));
            }

            biblioteca.agregarLibro(Libro(
                wstring_convert<codecvt_utf8<wchar_t>>().to_bytes(titulo),
                wstring_convert<codecvt_utf8<wchar_t>>().to_bytes(autor),
                wstring_convert<codecvt_utf8<wchar_t>>().to_bytes(fechaPublicacion),
                wstring_convert<codecvt_utf8<wchar_t>>().to_bytes(descripcion),
                categoriasList
            ));
        } else {
            wcerr << L"Error al procesar la línea: " << line << endl;
        }
    }
}

void guardarLibros(const Biblioteca& biblioteca) {
    wofstream file("libros.txt");
    file.imbue(locale(locale(), new codecvt_utf8<wchar_t>));
    
    for (const auto& libro : biblioteca.getLibros()) {
        file << wstring_convert<codecvt_utf8<wchar_t>>().from_bytes(libro.getTitulo()) << L","
             << wstring_convert<codecvt_utf8<wchar_t>>().from_bytes(libro.getAutor()) << L","
             << wstring_convert<codecvt_utf8<wchar_t>>().from_bytes(libro.getFechaPublicacion()) << L","
             << wstring_convert<codecvt_utf8<wchar_t>>().from_bytes(libro.getDescripcion()) << L",";
        
        const auto& categorias = libro.getCategorias();
        for (size_t i = 0; i < categorias.size(); ++i) {
            file << wstring_convert<codecvt_utf8<wchar_t>>().from_bytes(categorias[i]);
            if (i < categorias.size() - 1) {
                file << L";";
            }
        }
        file << L"\n";
    }
}

void cargarCuentas(Biblioteca& biblioteca) {
    ifstream file("cuentas.txt");
    if (!file.is_open()) {
        cerr << "Error: No se pudo abrir el archivo cuentas.txt" << endl;
        return;
    }
    string line, nombre, passwordHash, tipo;
    while (getline(file, line)) {
        istringstream ss(line);
        getline(ss, nombre, ',');
        getline(ss, passwordHash, ',');
        getline(ss, tipo);
        if (tipo == "admin") {
            biblioteca.agregarAdmin(Admin(nombre, "", passwordHash));
        } else if (tipo == "usuario") {
            biblioteca.agregarUsuario(Usuario(nombre, "", passwordHash));
        }
    }
}

void guardarCuentas(const Biblioteca& biblioteca) {
    ofstream file("cuentas.txt");
    for (const auto& admin : biblioteca.getAdmins()) {
        file << admin.getNombre() << ","
             << admin.getPasswordHash() << ",admin\n";
    }
    for (const auto& usuario : biblioteca.getUsuarios()) {
        file << usuario.getNombre() << ","
             << usuario.getPasswordHash() << ",usuario\n";
    }
}

void mostrarMenuAdmin() {
    cout << "Menu Admin:\n";
    cout << "1. Agregar libro\n";
    cout << "2. Editar libro\n";
    cout << "3. Eliminar libro\n";
    cout << "4. Ver estado de prestamos\n";
    cout << "5. Listar usuarios\n";
    cout << "6. Listar libros\n";
    cout << "0. Salir\n";
}

void mostrarMenuUsuario() {
    cout << "Menu Usuario:\n";
    cout << "1. Pedir prestado un libro\n";
    cout << "2. Devolver un libro\n";
    cout << "3. Listar libros\n";
    cout << "4. Filtrar libros por categoria\n";
    cout << "0. Salir\n";
}

void mostrarMenuInicial() {
    cout << "Bienvenido al sistema de biblioteca\n";
    cout << "1. Iniciar sesión\n";
    cout << "2. Crear nueva cuenta de usuario\n";
    cout << "0. Salir\n";
}

void crearNuevaCuenta(Biblioteca& biblioteca) {
    string nombre, password;
    cout << "Ingrese el nombre de usuario: ";
    cin >> nombre;
    cout << "Ingrese la contraseña: ";
    cin >> password;
    if (biblioteca.obtenerUsuario(nombre) || biblioteca.obtenerAdmin(nombre)) {
        cout << "El nombre de usuario ya existe. Por favor, elija otro.\n";
        return;
    }
    string passwordHash = hashPassword(password);
    Usuario nuevoUsuario(nombre, "", passwordHash);
    biblioteca.agregarUsuario(nuevoUsuario);
    guardarCuentas(biblioteca);
    cout << "Cuenta creada exitosamente.\n";
}

int main() {
    locale::global(locale(""));
    wcout.imbue(locale());
    Biblioteca biblioteca;
    cargarLibros(biblioteca);
    cargarCuentas(biblioteca);

    int opcionInicial;
    do {
        mostrarMenuInicial();
        cout << "Seleccione una opción: ";
        cin >> opcionInicial;
        cin.ignore();
        switch (opcionInicial) {
            case 1: { // Iniciar sesión
                string nombre, password;
                cout << "Nombre de usuario: ";
                cin >> nombre;
                cout << "Contraseña: ";
                cin >> password;
                Admin* admin = biblioteca.obtenerAdmin(nombre);
                Usuario* usuario = biblioteca.obtenerUsuario(nombre);

                if (admin && admin->verificarPassword(hashPassword(password))) {
                    int opcion;
                    do {
                        mostrarMenuAdmin();
                        cout << "Seleccione una opcion: ";
                        cin >> opcion;
                        cin.ignore(); // Clear the newline character from the input buffer
                        switch (opcion) {
                            case 1: {
                                string titulo, autor, fechaPublicacion, descripcion, categoria;
                                vector<string> categorias;
                                cout << "Titulo: ";
                                getline(cin, titulo);
                                cout << "Autor: ";
                                getline(cin, autor);
                                cout << "Fecha de publicacion: ";
                                getline(cin, fechaPublicacion);
                                cout << "Descripcion: ";
                                getline(cin, descripcion);
                                cout << "Categorias (separadas por ';'): ";
                                getline(cin, categoria);
                                
                                istringstream categoriasStream(categoria);
                                string cat;
                                while (getline(categoriasStream, cat, ';')) {
                                    categorias.push_back(cat);
                                }
                                admin->agregarLibro(biblioteca, Libro(titulo, autor, fechaPublicacion, descripcion, categorias));
                                break;
                            }               
                            case 2: {
                                admin->editarLibro(biblioteca);
                                break;
                            }
                            case 3: {
                                string titulo;
                                cout << "Titulo del libro a eliminar: ";
                                getline(cin, titulo);
                                admin->eliminarLibro(biblioteca, titulo);
                                break;
                            }
                            case 4:
                                admin->verEstadoPrestamos(biblioteca);
                                break;
                            case 5:
                                admin->listarUsuarios(biblioteca);
                                break;
                            case 6:
                                admin->mostrarLibros(biblioteca);
                                break;
                            case 0:
                                cout << "Saliendo...\n";
                                break;
                            default:
                                cout << "Opcion no valida.\n";
                                break;
                        }
                    } while (opcion != 0);
                } else if (usuario && usuario->verificarPassword((password))) {
                    int opcion;
                    do {
                        mostrarMenuUsuario();
                        cout << "Seleccione una opcion: ";
                        cin >> opcion;
                        cin.ignore(); 
                        switch (opcion) {
                            case 1: {
                                vector<string> titulosLibrosDisponibles = biblioteca.obtenerTitulosLibrosDisponibles();
                                if (titulosLibrosDisponibles.empty()) {
                                    cout << "No hay libros disponibles para préstamo en este momento.\n";
                                } else {
                                    cout << "Libros disponibles para préstamo:\n";
                                    for (size_t i = 0; i < titulosLibrosDisponibles.size(); ++i) {
                                        cout << i + 1 << ". " << titulosLibrosDisponibles[i] << "\n";
                                    }
                                    
                                    cout << "\nIngrese el número del libro que desea pedir prestado (0 para cancelar): ";
                                    int seleccion;
                                    cin >> seleccion;
                                    cin.ignore(); // Limpiar el buffer

                                    if (seleccion > 0 && seleccion <= static_cast<int>(titulosLibrosDisponibles.size())) {
                                        string titulo = titulosLibrosDisponibles[seleccion - 1];
                                        string fechaPrestamo, fechaDevolucion;
                                        
                                        cout << "Fecha de prestamo (YYYY-MM-DD): ";
                                        getline(cin, fechaPrestamo);
                                        cout << "Fecha de devolucion (YYYY-MM-DD): ";
                                        getline(cin, fechaDevolucion);
                                        
                                        usuario->solicitarPrestamo(biblioteca, titulo, fechaPrestamo, fechaDevolucion);
                                    } else if (seleccion != 0) {
                                        cout << "Selección inválida.\n";
                                    }
                                }
                                break;
                            }
                            case 2: {
                                vector<string> librosPrestados = usuario->obtenerLibrosPrestados();
                                if (librosPrestados.empty()) {
                                    cout << "No tienes libros prestados actualmente.\n";
                                } else {
                                    cout << "Tus libros prestados:\n";
                                    for (size_t i = 0; i < librosPrestados.size(); ++i) {
                                        cout << i + 1 << ". " << librosPrestados[i] << "\n";
                                    }
                                    cout << "\nIngrese el número del libro que desea devolver (0 para cancelar): ";
                                    int seleccion;
                                    cin >> seleccion;
                                    cin.ignore(); // Limpiar el buffer

                                    if (seleccion > 0 && seleccion <= static_cast<int>(librosPrestados.size())) {
                                        string tituloLibro = librosPrestados[seleccion - 1];
                                        size_t pos = tituloLibro.find(" (Prestado");
                                        if (pos != string::npos) {
                                            tituloLibro = tituloLibro.substr(0, pos);
                                        }
                                        usuario->devolverLibro(tituloLibro);
                                    } else if (seleccion != 0) {
                                        cout << "Selección inválida.\n";
                                    }
                                }
                                break;
                            }
                            case 3: {
                                usuario->listarLibros(biblioteca);
                                break;
                            }
                            case 4: {
                                usuario->filtrarLibrosPorCategoria(biblioteca);
                                break;
                            }
                            case 0:
                                cout << "Saliendo...\n";
                                break;
                            default:
                                cout << "Opcion no valida.\n";
                                break;
                        }
                    } while (opcion != 0);
                } else {
                    cout << "Credenciales incorrectas." << endl;
                }
                break;
            }
            case 2: // Crear nueva cuenta
                crearNuevaCuenta(biblioteca);
                break;
            case 0:
                cout << "Saliendo del programa...\n";
                break;
            default:
                cout << "Opción no válida.\n";
                break;
        }
    } while (opcionInicial != 0);

    guardarLibros(biblioteca);
    guardarCuentas(biblioteca);
    return 0;
}