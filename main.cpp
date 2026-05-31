#include <iostream>
#include "DISK.h"

using namespace std;

int main() {
    cout<<"SISTEMA GESTOR DE BASE DE DATOS"<<endl;
    bool existe_disco = 0;
    while (true) {
        cout<<"MENU DE OPCIONES \n 1. personalizar el disco \n 2. Insertar datos \n "
              "3. Buscar datos \n 4. Salir"<<endl;
        cout<<"INGRESE UNA OPCION: ";
        int opciones = 0;
        cin>>opciones;
        if (opciones < 1 || opciones > 5) {
            cout<<"opcion no valida, intente de nuevo :)"<<endl;
        }
        DISK* disco = nullptr;
        switch (opciones) {
            case 1:{
                if (!existe_disco) {
                    cout<<" PERSONALIZAR DISCO"<<endl;
                    cout<<"ADVERTENCIA: Este sistema funciona en base 2, solo ingrese \n"
                          "el exponente , el sistema automaticamente calculara las cantidades \n"
                          "para su disco"<<endl;
                    int platos = 0, pistas = 0, sectores = 0, capacidad = 0;
                    cout<<"Ingrese el nro de platos (Exponente)"<<endl;
                    cin>>platos;
                    cout<<"Ingrese el nro de pistas (Exponente)"<<endl;
                    cin>>pistas;
                    cout<<"Ingrese el nro de sectores (Exponente)"<<endl;
                    cin>>sectores;
                    cout<<"Ingrese la capacidad de cada sector (Exponente)"<<endl;
                    cin>>capacidad;
                    disco = new DISK(platos,pistas,sectores,capacidad);
                    disco->Formateador();
                }
                existe_disco = 1;
                bool opciones_disk = 0;
                cout<<"Activar opciones del disco (escribir 1 caso contrario 0): ";
                cin>>opciones_disk;
                cout<<"OPCIONES DEL DISCO"<<endl;
                cout<<"1. Eliminar disco\n"
                      "2. Volver el menu principal"<<endl;
                while (opciones_disk) {
                    int opciones_disco = 0;
                    cin>>opciones_disco;
                    switch (opciones_disco) {
                        case 1:
                            cout<<"ELIMINAR DISCO"<<endl;
                            delete disco;
                            existe_disco = 0;
                            opciones_disk = 0;
                            break;
                        case 2:
                            cout<<"MENU PRINCIPAL"<<endl;
                            opciones_disk = 0;
                            break;
                    }
                }
                break;}
            case 2:
                cout<<"INSERTAR DATOS (usar un csv)"<<endl;
                if (!existe_disco) {
                    cout<<"ERROR, cree un disco primero"<<endl;
                    break;
                }
                break;
            case 3:
                cout<<"BUSQUEDA DE DATOS"<<endl;
                if (!existe_disco) {
                    cout<<"ERROR, cree un disco primero"<<endl;
                    break;
                }
                break;
            case 4:
                return 0;
        }
    }
}