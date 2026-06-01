#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include "DISK.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <cstring>

using namespace std;

struct Registro {
    int id;
    char nombre[50];
    char email[100];
    float salario;
    
    string serializar() {
        stringstream ss;
        ss << id << "|" << nombre << "|" << email << "|" << salario;
        return ss.str();
    }
    
    void deserializar(const string& data) {
        stringstream ss(data);
        string temp;
        getline(ss, temp, '|');
        id = stoi(temp);
        getline(ss, nombre, '|');
        getline(ss, email, '|');
        getline(ss, temp, '|');
        salario = stof(temp);
    }
};

class DatabaseManager {
private:
    DISK* disco;
    int registros_por_sector;
    
public:
    DatabaseManager(DISK* d) : disco(d) {
        registros_por_sector = disco->getCapacidadSector() / 200; // 200 bytes aprox por registro
        if (registros_por_sector < 1) registros_por_sector = 1;
    }

    bool cargarCSV(const string& ruta_csv) {
        ifstream archivo_csv(ruta_csv);
        if (!archivo_csv.is_open()) {
            cout << "ERROR: No se pudo abrir el archivo CSV: " << ruta_csv << endl;
            return false;
        }
        
        string linea;
        int registro_actual = 0;
        int sector_actual = 0;
        string buffer_sector = "";

        getline(archivo_csv, linea);
        
        cout << "Cargando datos desde CSV..." << endl;
        
        while (getline(archivo_csv, linea)) {
            Registro reg;
            stringstream ss(linea);
            string temp;

            getline(ss, temp, ',');
            reg.id = stoi(temp);
            getline(ss, reg.nombre, ',');
            getline(ss, reg.email, ',');
            getline(ss, temp, ',');
            reg.salario = stof(temp);
            
            string registro_str = reg.serializar();
            
            registro_str.resize(200, ' ');
            
            buffer_sector += registro_str;
            registro_actual++;
  
            if (registro_actual >= registros_por_sector) {
                escribirSector(sector_actual, buffer_sector);
                sector_actual++;
                registro_actual = 0;
                buffer_sector = "";
            }
        }
        
        if (buffer_sector.length() > 0) {
            buffer_sector.resize(disco->getCapacidadSector(), ' ');
            escribirSector(sector_actual, buffer_sector);
        }
        
        archivo_csv.close();
        cout << "CSV cargado exitosamente. Se usaron " << sector_actual + 1 << " sectores." << endl;
        return true;
    }
  
    bool buscarPorID(int id_buscar) {
        long long capacidad = disco->getCapacidadSector();
        long long total_sectores = disco->getTotalSectores();
        char buffer[capacidad];
        
        for (long long s = 0; s < total_sectores; s++) {
         
            long long superficie = 0;
            long long pista = (s * capacidad) / (disco->getCapacidadSector() * disco->getTotalSectores());
            long long sector_num = s % disco->getCapacidadSector();
            
            disco->Leer_Sector(superficie, pista, sector_num, buffer);
            
            string contenido(buffer, capacidad);
            
            for (int i = 0; i < registros_por_sector; i++) {
                int inicio = i * 200;
                if (inicio + 200 > capacidad) break;
                
                string registro_str = contenido.substr(inicio, 200);
             
                registro_str.erase(registro_str.find_last_not_of(' ') + 1);
                
                if (registro_str.empty()) continue;
                
                Registro reg;
                reg.deserializar(registro_str);
                
                if (reg.id == id_buscar) {
                    cout << "\n=== REGISTRO ENCONTRADO ===" << endl;
                    cout << "ID: " << reg.id << endl;
                    cout << "Nombre: " << reg.nombre << endl;
                    cout << "Email: " << reg.email << endl;
                    cout << "Salario: " << reg.salario << endl;
                    cout << "Ubicado en sector: " << s << endl;
                    return true;
                }
            }
        }
        
        cout << "No se encontró el registro con ID: " << id_buscar << endl;
        return false;
    }

    void mostrarTodos() {
        long long capacidad = disco->getCapacidadSector();
        long long total_sectores = disco->getTotalSectores();
        char buffer[capacidad];
        bool encontrado = false;
        
        cout << "\n=== TODOS LOS REGISTROS ===" << endl;
        
        for (long long s = 0; s < total_sectores; s++) {
            long long superficie = 0;
            long long pista = 0;
            long long sector_num = s % disco->getCapacidadSector();
            
            disco->Leer_Sector(superficie, pista, sector_num, buffer);
            
            string contenido(buffer, capacidad);
            
            for (int i = 0; i < registros_por_sector; i++) {
                int inicio = i * 200;
                if (inicio + 200 > capacidad) break;
                
                string registro_str = contenido.substr(inicio, 200);
                registro_str.erase(registro_str.find_last_not_of(' ') + 1);
                
                if (registro_str.empty()) continue;
                
                Registro reg;
                reg.deserializar(registro_str);
                
                cout << "ID: " << reg.id << " | Nombre: " << reg.nombre 
                     << " | Email: " << reg.email << " | Salario: " << reg.salario << endl;
                encontrado = true;
            }
        }
        
        if (!encontrado) {
            cout << "No hay registros en el disco." << endl;
        }
    }
    
private:
    void escribirSector(int sector_num, const string& datos) {
       
        long long superficie = 0;
        long long pista = sector_num / disco->getCapacidadSector();
        long long sector = sector_num % disco->getCapacidadSector();
        
        disco->Escribir_sector(superficie, pista, sector, datos.c_str());
    }
};

#endif // DATABASE_MANAGER_H
