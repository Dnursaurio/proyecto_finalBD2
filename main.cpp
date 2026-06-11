#include <iostream>
#include <string>
#include "DISK.h"
#include "DatabaseManager.h"

using namespace std;

int main() {
    cout << "=== SISTEMA GESTOR DE BASE DE DATOS ===" << endl;
    bool existe_disco = false;
    DISK* disco = nullptr;
    DatabaseManager* dbManager = nullptr;
    
    while (true) {
        cout << "\n=== MENU DE OPCIONES ===" << endl;
        cout << "1. Personalizar y crear disco" << endl;
        cout << "2. Cargar archivo CSV" << endl;
        cout << "3. Mostrar todos los datos" << endl;
        cout << "4. Buscar dato (por ID o texto)" << endl;
        cout << "5. Estadísticas del disco" << endl;
        cout << "6. Eliminar disco" << endl;
        cout << "7. Salir" << endl;
        cout << "Ingrese opcion: ";
        
        int opcion = 0;
        cin >> opcion;
        
        switch (opcion) {
            case 1: {
                if (!existe_disco) {
                    cout << "\n=== CREAR DISCO ===" << endl;
                    int platos, pistas, sectores, capacidad;
                    cout << "Ingrese exponente para PLATOS (2^n): ";
                    cin >> platos;
                    cout << "Ingrese exponente para PISTAS (2^n): ";
                    cin >> pistas;
                    cout << "Ingrese exponente para SECTORES (2^n): ";
                    cin >> sectores;
                    cout << "Ingrese exponente para CAPACIDAD de sector en Bytes (2^n): ";
                    cin >> capacidad;
                    
                    disco = new DISK(platos, pistas, sectores, capacidad);
                    disco->Formateador();
                    dbManager = new DatabaseManager(disco);
                    existe_disco = true;
                    cout << "Disco creado exitosamente." << endl;
                } else {
                    cout << "Ya existe un disco. Elimínelo primero." << endl;
                }
                break;
            }
            
            case 2: {
                if (!existe_disco) {
                    cout << "Error: Cree un disco primero." << endl;
                    break;
                }
                string ruta;
                cout << "Ingrese la ruta del archivo CSV: ";
                cin >> ruta;
                dbManager->cargarCSV(ruta);
                break;
            }
            
            case 3: {
                if (!existe_disco) {
                    cout << "Error: Cree un disco primero." << endl;
                    break;
                }
                dbManager->mostrarTodos();
                break;
            }
            
            case 4: {
                if (!existe_disco) {
                    cout << "Error: Cree un disco primero." << endl;
                    break;
                }
                string texto;
                cout << "Ingrese el ID o texto a buscar: ";
                cin >> texto;
                dbManager->buscarTexto(texto);
                break;
            }
            
            case 5: {
                if (!existe_disco) {
                    cout << "Error: Cree un disco primero." << endl;
                    break;
                }
                dbManager->mostrarEstadisticas();
                break;
            }
            
            case 6: {
                if (existe_disco) {
                    delete disco;
                    delete dbManager;
                    disco = nullptr;
                    dbManager = nullptr;
                    existe_disco = false;
                    cout << "Disco eliminado." << endl;
                } else {
                    cout << "No hay disco para eliminar." << endl;
                }
                break;
            }
            
            case 7:
                cout << "Saliendo del sistema..." << endl;
                delete disco;
                delete dbManager;
                return 0;
                
            default:
                cout << "Opción no válida." << endl;
        }
    }
}
