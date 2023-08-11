//
// Created by leonf on 25/07/2023.
//
#include "Disco.h"
#ifndef BD2_BLOQUE_H
#define BD2_BLOQUE_H
#include "ModifySector.h"
#include <algorithm>
#include <cctype>
#include <fstream>

class Bloque{
public:
    vector<Sector *> sectores;
    Disco * disk;
    string data;
    int capacidadBloque;
    int memoriaDisponible;
    Bloque(Disco *_disk){
        data = "";
        disk = _disk;
        capacidadBloque = disk->memoriaSector*disk->sectorBloque;
    }
    void getInfoBloque(){
        memoriaDisponible = 0;
        int sectoresOcupados = 0;
        for(auto a:sectores){
            memoriaDisponible += a->memoriaDisponible;
            if(a->memoriaDisponible != a->capacidad) sectoresOcupados++;
        }
        sectoresOcupados == 0 ? cout<<"Estado bloque: Bloque vacio"<<endl : cout<<"Estado bloque: Bloque ocupado"<<endl;
        cout<<"Memoria total: "<<capacidadBloque<<endl;
        cout<<"Memoria disponible: "<<memoriaDisponible<<endl;
        cout<<"Numero de sectores: "<<sectores.size()<<endl;
        cout<<"Sectores ocupados: "<<sectoresOcupados<<endl;
        cout<<"Rutas de los sectores en disco: "<<endl;
        for(auto a:sectores){
            cout<<"\t"<<a->route<<endl;
        }
    }
    void writeDisk(const std::vector<ModifySector>& data) {
        for (const auto& i : data) {
            std::fstream archivo(sectores[i.sector]->route, std::ios::in | std::ios::out | std::ios::binary);

            if (!archivo) {
                std::cerr << "Error al abrir el archivo: " << sectores[i.sector]->route << std::endl;
                continue;
            }
            string dato = i.data;
            // Asegurarse de que el dato tenga el mismo tamaño que los demás registros, incluido el salto de línea
            int size = dato.size();
            int recordSize = 0;
            std::string linea;
            std::getline(archivo, linea);
            if (!linea.empty()) {
                recordSize = linea.size() + 1; // +1 para contar el salto de línea
            }
            if (size < recordSize) {
                dato += std::string(recordSize - size - 2, ' ');
            } else if (size > recordSize) {
                dato.resize(recordSize+1);
            }

            archivo.seekp((recordSize)*(i.linea - 1), std::ios::beg);
            dato += "\0";
            archivo.write(dato.c_str(), recordSize - 1); // Escribir el dato sin el salto de línea
            archivo.close();
        }
    }
    vector <string> cargarData(){
        vector<string> data;
        for(auto i: sectores){
            if(i != 0)
                data.push_back(i->getData());
        }
        return data;
    }
    string getKeyData(){
        string keys = "";
        for(auto i: sectores){
            if(i != 0)
                keys += i->getRegistro("id");
        }
        return keys;
    }

};

#endif //BD2_BLOQUE_H
