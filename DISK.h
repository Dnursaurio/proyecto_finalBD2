#ifndef PROYECTO_FINAL_BD2_DISK_H
#define PROYECTO_FINAL_BD2_DISK_H

#include <cmath>
#include <fstream>
#include <iostream>
#include <cstring>

using namespace std;

class DISK {
public:
    DISK(int p, int t, int s, int c) {
        nro_platos = pow(2, p);
        nro_superficies = 2 * nro_platos;
        nro_pistas = pow(2, t);
        nro_sectores = pow(2, s);
        capacidad_sector = pow(2, c);
    }

    ~DISK() {
        cout << "Disco Eliminado" << endl;
        remove("disco.dat");
    }

    void Formateador() {
        cout << "Creando su disco :)" << endl;
        long long capacidad_disco = nro_superficies * nro_pistas * nro_sectores * capacidad_sector;
        cout << "El disco tiene:\n"
             << " - " << nro_superficies << " superficies\n"
             << " - " << nro_pistas << " pistas\n"
             << " - " << nro_sectores << " sectores\n"
             << " - " << "cada sector pesa: " << capacidad_sector << " Bytes" << endl;
        cout << "La capacidad del disco es: " << capacidad_disco / 1024 << " KB" << endl;
        
        ofstream archivo("disco.dat", ios::out | ios::binary);
        archivo.seekp(capacidad_disco - 1);
        archivo.write("\0", 1);
        archivo.close();
    }

    void Escribir_sector(long long superficie, long long pista, long long sector, const char* buffer) {
        long long offset = Desplazamiento(superficie, pista, sector);
        fstream archivo("disco.dat", ios::in | ios::out | ios::binary);
        if (!archivo) {
            cout << "ERROR: No se pudo abrir el disco" << endl;
            return;
        }
        archivo.seekp(offset);
        archivo.write(buffer, capacidad_sector);
        archivo.close();
    }

    void Leer_Sector(long long superficie, long long pista, long long sector, char* buffer) {
        long long offset = Desplazamiento(superficie, pista, sector);
        fstream archivo("disco.dat", ios::in | ios::binary);
        if (!archivo) {
            cout << "ERROR: No se pudo abrir el disco" << endl;
            return;
        }
        archivo.seekg(offset);
        archivo.read(buffer, capacidad_sector);
        archivo.close();
    }

    long long getCapacidadSector() { return capacidad_sector; }
    long long getNroSuperficies() { return nro_superficies; }
    long long getNroPistas() { return nro_pistas; }
    long long getNroSectores() { return nro_sectores; }
    long long getTotalSectores() { return nro_superficies * nro_pistas * nro_sectores; }

private:
    long long nro_platos,
              nro_superficies,
              nro_pistas,
              nro_sectores,
              capacidad_sector;

    long long Desplazamiento(long long superficie, long long pista, long long sector) {
        long long sector_absoluto = (pista * nro_superficies + superficie) * nro_sectores + (sector - 1);
        return sector_absoluto * capacidad_sector;
    }
};

#endif //PROYECTO_FINAL_BD2_DISK_H
