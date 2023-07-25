//
// Created by leonf on 25/07/2023.
//
#include "Disco.h"
#ifndef BD2_BLOQUE_H
#define BD2_BLOQUE_H

class Bloque{
public:
    vector<Sector *> sectores;
    string data;
    Disco * disk;
    int memoria_bloque;
    Bloque(Disco *_disk){
        disk = _disk;
        data = "";
        memoria_bloque = disk->memoriaSector*disk->sectorBloque;
        sectores.resize(disk->sectorBloque,0);
    }
    void cargarData(){
        for(auto i: sectores){
            if(i != 0)
                data += i->getData();
        }
    }
    void getKeyData(){
        for(auto i: sectores){
            if(i != 0)
                data += i->getRegistro("id");
        }
    }

};


#endif //BD2_BLOQUE_H
