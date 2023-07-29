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
    void writeDisk(string data) {
        int temp = 0;
        int memoriaMax = sectores[0]->capacidad;
        int memoriaActual = memoriaMax;
        istringstream iss(data);
        string line;

        // Abrir el archivo fuera del bucle para evitar reemplazar su contenido
        ofstream archivo(sectores[temp]->route);

        while (getline(iss, line)) {
            int mDisponible = memoriaActual - line.size();
            if (mDisponible >= 0) {
                archivo << line << endl;
                memoriaActual -= line.size() + 1; // +1 para contar el carácter de nueva línea
            } else {
                archivo.close(); // Cerrar el archivo actual antes de cambiar al siguiente
                temp++;
                if (temp >= sectores.size()) {
                    cout << "No hay más sectores disponibles." << endl;
                    return;
                }
                archivo.open(sectores[temp]->route);
                memoriaActual = memoriaMax;
                // No debemos olvidarnos de escribir la línea actual en el nuevo archivo abierto
                archivo << line << endl;
                memoriaActual -= line.size() + 1; // +1 para contar el carácter de nueva línea
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
