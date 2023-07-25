//
// Created by leonf on 25/07/2023.
//

#ifndef BD2_BUFFERMANAGER_H
#define BD2_BUFFERMANAGER_H

#include <iostream>
#include <vector>
#include <unordered_map>

class BufferManager {
private:
    struct Page {
        int page_id;
        bool referenced;
        bool dirty;
        // Aquí puedes agregar más información necesaria para la página, como los datos almacenados, etc.
    };

    int buffer_size;
    std::vector<Page> buffer;
    std::unordered_map<int, int> page_map; // Para mapear el page_id a su índice en el buffer.
    int hand; // Índice del puntero "manecilla" para el algoritmo de reemplazo "clock".

public:
    BufferManager(int size) : buffer_size(size), hand(0) {
        buffer.resize(buffer_size);
    }

    void readPage(int page_id) {
        int index = getPageIndex(page_id);
        if (index != -1) {
            buffer[index].referenced = true;
            std::cout << "Reading page " << page_id << " from buffer." << std::endl;
        } else {
            // La página no está en el buffer, la cargamos.
            std::cout << "Page " << page_id << " not found in buffer. Loading it." << std::endl;

            // Aquí implementarías la lógica para cargar la página desde el almacenamiento,
            // pero para este ejemplo, solo simularemos que se carga en el buffer.
            Page new_page;
            new_page.page_id = page_id;
            new_page.referenced = true;
            new_page.dirty = false;
            buffer[hand] = new_page;
            page_map[page_id] = hand;

            hand = (hand + 1) % buffer_size; // Movemos la manecilla al siguiente índice.
        }
    }

    void writePage(int page_id) {
        int index = getPageIndex(page_id);
        if (index != -1) {
            buffer[index].referenced = true;
            buffer[index].dirty = true;
            std::cout << "Writing page " << page_id << " to buffer." << std::endl;
        } else {
            // La página no está en el buffer, la cargamos y marcamos como dirty.
            std::cout << "Page " << page_id << " not found in buffer. Loading it." << std::endl;

            // Aquí implementarías la lógica para cargar la página desde el almacenamiento,
            // pero para este ejemplo, solo simularemos que se carga en el buffer.
            Page new_page;
            new_page.page_id = page_id;
            new_page.referenced = true;
            new_page.dirty = true;
            buffer[hand] = new_page;
            page_map[page_id] = hand;

            hand = (hand + 1) % buffer_size; // Movemos la manecilla al siguiente índice.
        }
    }

    // Método para obtener el índice de una página en el buffer.
    int getPageIndex(int page_id) {
        auto it = page_map.find(page_id);
        if (it != page_map.end()) {
            return it->second;
        }
        return -1;
    }
};



#endif //BD2_BUFFERMANAGER_H
