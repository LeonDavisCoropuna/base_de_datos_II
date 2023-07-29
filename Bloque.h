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
    int memoria_bloque;
    Bloque(Disco *_disk){
        data = "";
        disk = _disk;
        memoria_bloque = disk->memoriaSector*disk->sectorBloque;
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
                dato += std::string(recordSize - size - 3, ' ');
            } else if (size > recordSize) {
                dato.resize(recordSize - 1);
            }

            // Moverse a la posición de la línea a modificar
            archivo.seekp((recordSize)*(i.linea - 1), std::ios::beg);
            //for(int k=0;k<dato.length()+1;k++) cout<<"-"<<int(dato[k])<<"-";
            //dato = dato.substr(0,dato.size()-1);
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
