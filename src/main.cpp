#include <iostream>
#include <locale>
#include "Admin.h"
#include "Usuario.h"
#include "Biblioteca.h"
#include "Menu.h"
#include "GestorDatos.h"
#include "Utils.h"

using namespace std;

// void cargarLibros(Biblioteca& biblioteca) {
//     wifstream file("libros.txt");
//     file.imbue(locale(locale(), new codecvt_utf8<wchar_t>));
    
//     if (!file.is_open()) {
//         cerr << "Error: No se pudo abrir el archivo libros.txt" << endl;
//         return;
//     }

//     wstring line;
//     while (getline(file, line)) {
//         wstringstream ss(line);
//         wstring titulo, autor, fechaPublicacion, descripcion, categorias;

//         if (getline(ss, titulo, L',') &&
//             getline(ss, autor, L',') &&
//             getline(ss, fechaPublicacion, L',') &&
//             getline(ss, descripcion, L',') &&
//             getline(ss, categorias)) {

//             vector<string> categoriasList;
//             wstringstream categoriasStream(categorias);
//             wstring categoria;
//             while (getline(categoriasStream, categoria, L';')) {
//                 categoriasList.push_back(wstring_convert<codecvt_utf8<wchar_t>>().to_bytes(categoria));
//             }

//             biblioteca.agregarLibro(Libro(
//                 wstring_convert<codecvt_utf8<wchar_t>>().to_bytes(titulo),
//                 wstring_convert<codecvt_utf8<wchar_t>>().to_bytes(autor),
//                 wstring_convert<codecvt_utf8<wchar_t>>().to_bytes(fechaPublicacion),
//                 wstring_convert<codecvt_utf8<wchar_t>>().to_bytes(descripcion),
//                 categoriasList
//             ));
//         } else {
//             wcerr << L"Error al procesar la línea: " << line << endl;
//         }
//     }
// }

// void guardarLibros(const Biblioteca& biblioteca) {
//     wofstream file("libros.txt");
//     file.imbue(locale(locale(), new codecvt_utf8<wchar_t>));
    
//     for (const auto& libro : biblioteca.getLibros()) {
//         file << wstring_convert<codecvt_utf8<wchar_t>>().from_bytes(libro.getTitulo()) << L","
//              << wstring_convert<codecvt_utf8<wchar_t>>().from_bytes(libro.getAutor()) << L","
//              << wstring_convert<codecvt_utf8<wchar_t>>().from_bytes(libro.getFechaPublicacion()) << L","
//              << wstring_convert<codecvt_utf8<wchar_t>>().from_bytes(libro.getDescripcion()) << L",";
        
//         const auto& categorias = libro.getCategorias();
//         for (size_t i = 0; i < categorias.size(); ++i) {
//             file << wstring_convert<codecvt_utf8<wchar_t>>().from_bytes(categorias[i]);
//             if (i < categorias.size() - 1) {
//                 file << L";";
//             }
//         }
//         file << L"\n";
//     }
// }

// void cargarCuentas(Biblioteca& biblioteca) {
//     ifstream file("cuentas.txt");
//     if (!file.is_open()) {
//         cerr << "Error: No se pudo abrir el archivo cuentas.txt" << endl;
//         return;
//     }
//     string line, nombre, passwordHash, tipo;
//     while (getline(file, line)) {
//         istringstream ss(line);
//         getline(ss, nombre, ',');
//         getline(ss, passwordHash, ',');
//         getline(ss, tipo);
//         if (tipo == "admin") {
//             biblioteca.agregarAdmin(Admin(nombre, "", passwordHash));
//         } else if (tipo == "usuario") {
//             biblioteca.agregarUsuario(Usuario(nombre, "", passwordHash));
//         }
//     }
// }

// void guardarCuentas(const Biblioteca& biblioteca) {
//     ofstream file("cuentas.txt");
//     for (const auto& admin : biblioteca.getAdmins()) {
//         file << admin.getNombre() << ","
//              << admin.getPasswordHash() << ",admin\n";
//     }
//     for (const auto& usuario : biblioteca.getUsuarios()) {
//         file << usuario.getNombre() << ","
//              << usuario.getPasswordHash() << ",usuario\n";
//     }
// }

// void crearNuevaCuenta(Biblioteca& biblioteca) {
//     string nombre, password;
//     cout << "Ingrese el nombre de usuario: ";
//     cin >> nombre;
//     cout << "Ingrese la contraseña: ";
//     cin >> password;
//     if (biblioteca.obtenerUsuario(nombre) || biblioteca.obtenerAdmin(nombre)) {
//         cout << "El nombre de usuario ya existe. Por favor, elija otro.\n";
//         return;
//     }
//     string passwordHash = hashPassword(password);
//     Usuario nuevoUsuario(nombre, "", passwordHash);
//     biblioteca.agregarUsuario(nuevoUsuario);
//     guardarCuentas(biblioteca);
//     cout << "Cuenta creada exitosamente.\n";
// }

int main() {
    locale::global(locale(""));
    wcout.imbue(locale());
    Biblioteca biblioteca;
    GestorDatos::cargarLibros(biblioteca);
    GestorDatos::cargarCuentas(biblioteca);

    int opcionInicial;
    do {
        Menu::mostrarMenuPrincipal();
        opcionInicial = Menu::obtenerOpcion();
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
                    Menu::manejarMenuAdmin(admin, biblioteca);
                } else if (usuario && usuario->verificarPassword((password))) {
                    Menu::manejarMenuUsuario(usuario, biblioteca);
                } else {
                    cout << "Credenciales incorrectas." << endl;
                }
                break;
            }
            case 2: // Crear nueva cuenta
                GestorDatos::crearNuevaCuenta(biblioteca);
                break;
            case 3:
                cout << "Saliendo del programa...\n";
                break;
            default:
                Menu::mostrarMensajeError();
                break;
        }
    } while (opcionInicial != 3);

    GestorDatos::guardarLibros(biblioteca);
    GestorDatos::guardarCuentas(biblioteca);
    return 0;
}