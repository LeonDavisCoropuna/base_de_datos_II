//
// Created by leonf on 29/07/2023.
//

#ifndef BD2_MODIFYSECTOR_H
#define BD2_MODIFYSECTOR_H
#include <iostream>
struct ModifySector{
    std::string data;
    bool dirty = 0;
    int sector;
    int linea;
};


#endif //BD2_MODIFYSECTOR_H
