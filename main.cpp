#include <iostream>
#include <string>
#include "DISK.h"
#include "DatabaseManager.h"

using namespace std;

int main() {
    cout << "SISTEMA GESTOR DE BASE DE DATOS" << endl;
    bool existe_disco = false;
    DISK* disco = nullptr;
    DatabaseManager* dbManager = nullptr;
    
    while (true) {
        cout << "\nMENU DE OPCIONES" << endl;
        cout << "1. Personalizar el disco" << endl;
        cout << "2. Insertar datos (cargar CSV)" << endl;
        cout << "3. Buscar datos (por ID)" << endl;
        cout << "4. Mostrar todos los registros" << endl;
        cout << "5. Salir" << endl;
        cout << "INGRESE UNA OPCION: ";
        
        int opciones = 0;
        cin >> opciones;
        
        if (opciones < 1 || opciones > 5) {
            cout << "Opcion no valida, intente de nuevo :)" << endl;
            continue;
        }
        
        switch (opciones) {
            case 1: {
                if (!existe_disco) {
                    cout << "\n=== PERSONALIZAR DISCO ===" << endl;
                    cout << "ADVERTENCIA: Este sistema funciona en base 2, solo ingrese el exponente" << endl;
                    cout << "El sistema automaticamente calculara las cantidades para su disco" << endl;
                    
                    int platos = 0, pistas = 0, sectores = 0, capacidad = 0;
                    
                    cout << "Ingrese el nro de platos (Exponente): ";
                    cin >> platos;
                    cout << "Ingrese el nro de pistas (Exponente): ";
                    cin >> pistas;
                    cout << "Ingrese el nro de sectores (Exponente): ";
                    cin >> sectores;
                    cout << "Ingrese la capacidad de cada sector (Exponente en Bytes): ";
                    cin >> capacidad;
                    
                    disco = new DISK(platos, pistas, sectores, capacidad);
                    disco->Formateador();
                    existe_disco = true;
                    
                    // Inicializar el DatabaseManager
                    dbManager = new DatabaseManager(disco);
                } else {
                    cout << "Ya existe un disco creado." << endl;
                }
                
                // Menú de opciones del disco
                int opciones_disk = 0;
                cout << "\nOPCIONES DEL DISCO" << endl;
                cout << "1. Eliminar disco" << endl;
                cout << "2. Volver al menu principal" << endl;
                cout << "Ingrese opcion: ";
                cin >> opciones_disk;
                
                if (opciones_disk == 1) {
                    cout << "ELIMINAR DISCO" << endl;
                    delete disco;
                    delete dbManager;
                    disco = nullptr;
                    dbManager = nullptr;
                    existe_disco = false;
                }
                break;
            }
            
            case 2: {
                cout << "\n=== INSERTAR DATOS (CSV) ===" << endl;
                if (!existe_disco) {
                    cout << "ERROR: Cree un disco primero." << endl;
                    break;
                }
                
                string ruta_csv;
                cout << "Ingrese la ruta del archivo CSV: ";
                cin >> ruta_csv;
                
                if (dbManager->cargarCSV(ruta_csv)) {
                    cout << "Datos cargados exitosamente." << endl;
                }
                break;
            }
            
            case 3: {
                cout << "\n=== BUSQUEDA DE DATOS ===" << endl;
                if (!existe_disco) {
                    cout << "ERROR: Cree un disco primero." << endl;
                    break;
                }
                
                int id_buscar;
                cout << "Ingrese el ID a buscar: ";
                cin >> id_buscar;
                
                dbManager->buscarPorID(id_buscar);
                break;
            }
            
            case 4: {
                cout << "\n=== MOSTRAR TODOS LOS REGISTROS ===" << endl;
                if (!existe_disco) {
                    cout << "ERROR: Cree un disco primero." << endl;
                    break;
                }
                
                dbManager->mostrarTodos();
                break;
            }
            
            case 5:
                cout << "Saliendo del sistema..." << endl;
                delete disco;
                delete dbManager;
                return 0;
        }
    }
}
