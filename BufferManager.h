//
// Created by leonf on 25/07/2023.
//

#ifndef BD2_BUFFERMANAGER_H
#define BD2_BUFFERMANAGER_H
#include <iostream>
#include <vector>
#include "Bloque.h"
#include "DiskManager.h"
#include <list>
#include <algorithm>
#include "ModifySector.h"
using namespace std;


class NodoRegistro{
public:
    string registro;
    int linea;
    int sizeRegistro;
    bool empty;
};

class Page {
public:
    int bit_uso;
    int pin_count; // Indica si la página está siendo utilizada por algún proceso
    bool dirty_bit; // Indica si la página ha sido modificada y necesita escribirse en disco
    int frame_id;   // Identificador del marco de la página en el buffer
    int pageId;     // Identificador único de la página
    Bloque *bloq;
    using ListIterator = vector<ModifySector>::iterator;

    vector <vector<ModifySector>> linkedList;
    list <int> freeSpace;

    Page(int _pageId) : pageId(_pageId), bit_uso(1), pin_count(1), dirty_bit(false), frame_id(-1) {}
    void setBloq(Bloque * bloque){
        bloq = bloque;
        generateList();
    }
    ListIterator findRecord(string key) {
        int sizeKeyMagic = 5;
        for (int i = key.size(); i < sizeKeyMagic; i++) {
            key += " ";
        }
        int pos_record_file = 1;
        int pos_record_bloq = 0;
        for (auto it = linkedList.begin(); it != linkedList.end(); ++it) {
            for (auto element = it->begin(); element != it->end(); ++element) {
                if (element->data.substr(0, key.size()) == key) {
                    element->linea = pos_record_file;
                    element->sector = pos_record_bloq;
                    return element; // Devolvemos un iterador que apunta al string encontrado dentro de registros
                }
                pos_record_file++;
            }
            pos_record_file = 1;
            pos_record_bloq++;
        }
        return linkedList.end()->end(); // Si no se encuentra, devolvemos el iterador end() que indica "no encontrado"
    }
    void Insertar(string record){
        std::istringstream iss(record);
        std::vector<std::string> partes;
        std::string palabra;
        while (std::getline(iss, palabra, '#')) {
            partes.push_back(palabra);
        }
        auto *it = &linkedList[stoi(partes[0])][stoi(partes[1])];
        (*it).data = partes[2];
        (*it).linea = stoi(partes[1]);
        (*it).dirty = 1;
        (*it).sector =stoi(partes[0]) ;

    }
    void Eliminar(string idRegistro){
        ListIterator it = findRecord(idRegistro);
        (*it).dirty = 1;
        (*it).data = "vacio";
    }
    void generateList() {
        vector<string> data = bloq->cargarData();
        size_t start = 0;
        size_t end = 0;
        for (auto i : data) {
            vector<ModifySector> registros;
            while (end < i.size()) { // Cambiado de i[end] != '\0' a end < i.size()
                if (i[end] == '\n') {
                    string value = string(i.begin() + start, i.begin() + end); // Cambiado de i + start a i.begin() + start
                    registros.push_back(ModifySector {value,0});
                    start = end + 1;
                }
                end++;
            }
            if (end > start) {
                string value = string(i.begin() + start, i.begin() + end); // Cambiado de data + start a i.begin() + start
                registros.push_back(ModifySector {value,0});
            }
            linkedList.push_back(registros);
            start = 0; // Reiniciar los valores para el siguiente registro
            end = 0;
        }
    }
};

class BufferManager {
public:
    int buffer_size;
    int puntero = 0;
    int pageCounter=0;
    std::vector<Page*> buffer;

    BufferManager(int size) : buffer_size(size), puntero(0) {}

    Page* findPage(int idPage) {
        for (auto i : buffer) {
            if (i->pageId == idPage) return i;
        }
        return nullptr;
    }
    // Función para reemplazar una página en el buffer (política Clock)
    void writeDisk(Page *page){
        vector<ModifySector> registros;
        for(auto i: page->linkedList){
            for(auto j: i){
                if(j.dirty) registros.push_back(j);
            }
        }
        page->bloq->writeDisk(registros);
        cout<<"Escribir en disco"<<endl;
    }
    void setBloq(int pageId,Bloque *bl) {
        Page *page = findPage(pageId);
        page->setBloq(bl);
    }
    void insertPage(int idPage,Bloque *bloq) {
        Page* page = findPage(idPage);
        if (page == nullptr) {
            page = new Page(idPage);
            page->setBloq(bloq);
            page->bit_uso = 1;
            while (true) {
                if(pageCounter > buffer_size){
                    cout<<"Es necesario liberar alguna pagina"<<endl;
                    break;
                }
                if (buffer.size() < buffer_size) {
                    buffer.push_back(page);
                    page->frame_id = buffer.size() - 1; // Establecer frame_id a la posición actual
                    break;
                } else {
                    if(buffer[puntero]->pin_count <= 0){
                        if (buffer[puntero]->bit_uso) {
                            buffer[puntero]->bit_uso = 0;
                            puntero = (puntero + 1) % (buffer_size);
                        } else {
                            // Reemplazar página usando la política Clock

                            if(buffer[puntero]->dirty_bit){
                                writeDisk(buffer[puntero]);
                            }

                            buffer[puntero] = page;
                            page->frame_id = puntero; // Establecer frame_id a la posición actual
                            puntero = (puntero + 1) % (buffer_size);
                            break;
                        }
                        pageCounter = 0;
                    }
                    else{
                        puntero = (puntero + 1) % (buffer_size);
                        pageCounter++;
                    }

                }
            }
        } else {
            findPage(idPage)->pin_count++;
            page->setBloq(bloq);
            page->bit_uso = 1;
        }
    }
    void modifyPage(int idPage,int opt,string record){
        Page *page = findPage(idPage);
        if(page){
            page->dirty_bit = 1;
        }
        else{
            return;
        }
        switch (opt) {
            case 1:
                //insertar
                page->Insertar(record);
                cout<<"Regsitro insertado"<<endl;
                break;
            case 2:
                //eliminar
                page->Eliminar(record);
                cout<<"Registro eliminado"<<endl;
                break;
        }

    }
    void realeasePage(int idPage){
        Page *page = findPage(idPage);
        if(page)
            page->pin_count--;
        if(page->pin_count == 0){
            deletePage(idPage);
        }
    }
    void deletePage(int idPage){
        Page *page = findPage(idPage);
        if(page){
            page->pin_count = 0;
            if(page->dirty_bit) writeDisk(page);
            page->dirty_bit = 0;
        }
    }
    void printStateBuffer() {
        for (auto i : buffer) {
            cout << "Frame ID: " << i->frame_id << " | ";
            cout << "Page ID: " << i->pageId << " | ";
            cout << "Bit Uso: " << i->bit_uso << " | ";
            cout << "Pin Count: " << i->pin_count << " | ";
            cout << "Dirty Bit: " << i->dirty_bit << endl;
        }
    }
};

#endif //BD2_BUFFERMANAGER_H
