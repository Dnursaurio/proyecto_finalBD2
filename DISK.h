#ifndef PROYECTO_FINAL_BD2_DISK_H
#define PROYECTO_FINAL_BD2_DISK_H

#include  <cmath>
#include <fstream>
#include <iostream>

using namespace std;

class DISK {
public:
    DISK(int p, int t, int s, int c) {
        nro_platos = pow(2,p);
        nro_superficies = 2 * nro_platos;
        nro_pistas = pow(2,t);
        nro_sectores = pow(2,s);
        capacidad_sector = pow(2,c);
    }

    ~DISK() {
        cout<<"Disco Eliminado"<<endl;
        remove("disco.dat");
    }

    void Formateador() {
        cout<<"Creando su disco :)"<<endl;
        long long capacidad_disco = nro_superficies * nro_pistas * nro_sectores * capacidad_sector;
        cout<<"el tisco tiene:\n"
        <<" - "<< nro_superficies << " superficies\n"
        <<" - "<< nro_pistas <<" pistas \n"
        <<" - "<< nro_sectores << " sectores\n"
        <<" - "<<"cada sector pesa: " << capacidad_sector << " Bytes"<<endl;
        cout<<"la capacidad del disco es: " << capacidad_disco / 1024 << " KB"<<endl;
        ofstream archivo("disco.dat",ios::out|ios::binary);
        archivo.seekp(capacidad_disco - 1);
        archivo.write("\0",1);
        archivo.close();
    }
    void Escribir_sector() {

    }
    void Leer_Sector();

private:
    long long nro_platos,
    nro_superficies,
    nro_pistas,
    nro_sectores,
    capacidad_sector;

    long long Desplazamiento(long long superficie, long long pista, long long sector) {
        long long sectores_prev = (pista * nro_pistas + superficie) * nro_sectores + (sector - 1);
        return sectores_prev * capacidad_sector;
    }
};

#endif //PROYECTO_FINAL_BD2_DISK_H
