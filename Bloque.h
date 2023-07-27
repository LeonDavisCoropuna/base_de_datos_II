//
// Created by leonf on 25/07/2023.
//
#include "Disco.h"
#ifndef BD2_BLOQUE_H
#define BD2_BLOQUE_H

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
    void writeDisk(string data){
        int temp = 0;
        int memoriaMax = sectores[0]->capacidad;
        int memoriaActual = memoriaMax;
        istringstream iss(data);
        string line;
        ofstream archivo(sectores[temp]->route);
        while (getline(iss, line)) {
            int mDisponible = memoriaActual - line.size();
            if(mDisponible > 0){
                archivo<<line<<endl;
                memoriaActual = memoriaActual - line.size();
            }
            else{
                memoriaActual = memoriaMax;
                temp++;
                archivo.open(sectores[temp]->route);
            }
        }
    }
    const char *cargarData(){
        for(auto i: sectores){
            if(i != 0)
                data += i->getData();
        }
        return data.c_str();
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
