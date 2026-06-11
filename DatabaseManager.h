#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include "DISK.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <cstring>

using namespace std;

class DatabaseManager {
private:
    DISK* disco;
    long long registros_por_sector;
    vector<string> encabezados;
    long long sector_actual;
    string buffer_sector;
    int registros_en_buffer;
    
public:
    DatabaseManager(DISK* d) : disco(d) {
        registros_por_sector = disco->getCapacidadSector() / 256;
        if (registros_por_sector < 1) registros_por_sector = 1;
        sector_actual = 0;
        registros_en_buffer = 0;
        buffer_sector = "";
    }

    bool cargarCSV(const string& ruta_csv) {
        ifstream archivo_csv(ruta_csv);
        if (!archivo_csv.is_open()) {
            cout << "ERROR: No se pudo abrir el archivo CSV: " << ruta_csv << endl;
            return false;
        }
        
        string linea;
        int linea_num = 0;
   
        if (getline(archivo_csv, linea)) {
            stringstream ss(linea);
            string columna;
            encabezados.clear();
            while (getline(ss, columna, ',')) {
                encabezados.push_back(columna);
            }
            cout << "Encabezados detectados: ";
            for (const auto& h : encabezados) {
                cout << h << " ";
            }
            cout << endl;
            linea_num++;
        }
        
        cout << "\nCargando datos desde CSV..." << endl;

        while (getline(archivo_csv, linea)) {
            string registro = linea + "\n";
            buffer_sector += registro;
            registros_en_buffer++;
            
            if (registros_en_buffer >= registros_por_sector || 
                buffer_sector.length() >= disco->getCapacidadSector() - 256) {
                escribirSectorActual();
            }
            linea_num++;
        }
        
        if (buffer_sector.length() > 0) {
            escribirSectorActual();
        }
        
        archivo_csv.close();
        cout << "CSV cargado exitosamente. " << (linea_num - 1) << " registros cargados en " 
             << sector_actual << " sectores." << endl;
        return true;
    }

    bool buscarTexto(const string& texto_buscar) {
        long long capacidad = disco->getCapacidadSector();
        long long total_sectores = disco->getTotalSectores();
        char* buffer = new char[capacidad + 1];
        bool encontrado = false;
        
        cout << "\n=== BUSCANDO: \"" << texto_buscar << "\" ===" << endl;
        
        for (long long s = 0; s < total_sectores; s++) {
            long long superficie = 0;
            long long pista = (s * capacidad) / (disco->getCapacidadSector() * total_sectores);
            long long sector_num = s % disco->getCapacidadSector();
            
            disco->Leer_Sector(superficie, pista, sector_num, buffer);
            buffer[capacidad] = '\0';
            
            string contenido(buffer);
            
            size_t pos = 0;
            int linea_num = 1;
            while ((pos = contenido.find(texto_buscar, pos)) != string::npos) {
                size_t inicio_linea = contenido.rfind('\n', pos);
                if (inicio_linea == string::npos) inicio_linea = 0;
                else inicio_linea++;
                
                size_t fin_linea = contenido.find('\n', pos);
                string linea = contenido.substr(inicio_linea, fin_linea - inicio_linea);
                
                cout << "Sector " << s << ", Línea " << linea_num << ": " << linea << endl;
                encontrado = true;
                pos = fin_linea;
                linea_num++;
            }
        }
        
        if (!encontrado) {
            cout << "No se encontró el texto: \"" << texto_buscar << "\"" << endl;
        }
        
        delete[] buffer;
        return encontrado;
    }

    void mostrarTodos() {
        long long capacidad = disco->getCapacidadSector();
        long long total_sectores = disco->getTotalSectores();
        char* buffer = new char[capacidad + 1];
        bool hay_datos = false;
        
        cout << "\n=== TODOS LOS REGISTROS ===" << endl;
        
        if (!encabezados.empty()) {
            for (const auto& h : encabezados) {
                cout << h << "\t";
            }
            cout << "\n" << string(50, '-') << endl;
        }
        
        for (long long s = 0; s < total_sectores; s++) {
            long long superficie = 0;
            long long pista = (s * capacidad) / (disco->getCapacidadSector() * total_sectores);
            long long sector_num = s % disco->getCapacidadSector();
            
            disco->Leer_Sector(superficie, pista, sector_num, buffer);
            buffer[capacidad] = '\0';
            
            string contenido(buffer);
            stringstream ss(contenido);
            string linea;
            while (getline(ss, linea, '\n')) {
                if (!linea.empty()) {
                    for (char& c : linea) {
                        if (c == ',') c = '\t';
                    }
                    cout << linea << endl;
                    hay_datos = true;
                }
            }
        }
        
        if (!hay_datos) {
            cout << "No hay registros en el disco." << endl;
        }
        
        delete[] buffer;
    }

    bool buscarPorID(const string& id_buscar) {
        return buscarTexto(id_buscar);
    }

    void mostrarEstadisticas() {
        cout << "\n=== ESTADÍSTICAS DEL DISCO ===" << endl;
        cout << "Capacidad por sector: " << disco->getCapacidadSector() << " Bytes" << endl;
        cout << "Total de sectores: " << disco->getTotalSectores() << endl;
        cout << "Registros por sector (aprox): " << registros_por_sector << endl;
        cout << "Sectores utilizados: " << sector_actual << endl;
        
        long long capacidad_total = disco->getTotalSectores() * disco->getCapacidadSector();
        long long capacidad_usada = sector_actual * disco->getCapacidadSector();
        double porcentaje = (double)capacidad_usada / capacidad_total * 100;
        
        cout << "Espacio usado: " << capacidad_usada / 1024 << " KB / " 
             << capacidad_total / 1024 << " KB (" << porcentaje << "%)" << endl;
    }
    
private:
    void escribirSectorActual() {
        if (buffer_sector.empty()) return;
        
        if (buffer_sector.length() < disco->getCapacidadSector()) {
            buffer_sector.resize(disco->getCapacidadSector(), '\0');
        }
        
        long long superficie = 0;
        long long pista = sector_actual / disco->getNroSectores();
        long long sector_num = sector_actual % disco->getNroSectores();
        
        disco->Escribir_sector(superficie, pista, sector_num, buffer_sector.c_str());
        
        sector_actual++;
        buffer_sector = "";
        registros_en_buffer = 0;
    }
};

#endif // DATABASE_MANAGER_H
