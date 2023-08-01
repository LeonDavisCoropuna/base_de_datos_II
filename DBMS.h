//
// Created by leonf on 25/07/2023.
//

#ifndef BD2_DBMS_H
#define BD2_DBMS_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include "BufferManager.h"
#include "DiskManager.h"
#include "BTree.h"
#include <sstream>

class DBMS{
public:
    BufferManager *BM;
    DiskManager * DM;

    DBMS(DiskManager *dm, BufferManager* bm){
        DM = dm;
        BM = bm;
    }
    string getFreeSpace(){
        string data = DM->disk->getFreeSpace();
        DM->disk->updateFreeSpace();
        return data;
    }
    void sql_Request(string data,int opt)
    {
        // idRegistro       route                                            bloque
        // 100              titanic/plato2/superficie1/pista4/sector5           3
        int nroBloque = 1; //esto retorna el bTree
        Bloque *bloq = DM->directorioBloques[nroBloque];

        BM->insertPage(nroBloque,bloq);
        //BM->modifyPage(nroBloque,2);

        switch (opt) {
            case 1:
                //select
                break;
            case 2: {
                string freeSpaceDisk = getFreeSpace();
                std::istringstream iss(freeSpaceDisk);
                string bloq;
                string bloqSector;
                string lineSector;
                iss >> bloq;
                iss >> bloqSector;
                iss >> lineSector;

                string dataInsert = bloqSector + "#" + lineSector + "#" + data;
                cout<<dataInsert<<endl;
                BM->modifyPage(stoi(bloq), 1, dataInsert);
                BM->deletePage(stoi(bloq));
                break;
            }
            case 3: {//eliminar
                BM->modifyPage(nroBloque, 2, data);
                BM->deletePage(nroBloque);
                break;
            }
        }
        //BM->modifyPage(nroBloque,1);
        //BM->deletePage(nroBloque);
        BM->printStateBuffer();

    }
};


#endif //BD2_DBMS_H
