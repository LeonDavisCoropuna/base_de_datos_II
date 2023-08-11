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

        //DM->disk->updateFreeSpace();
        return data;
    }
    string getLastSpace(){
        return DM->disk->getLastSpace();
    }

    void sql_Request(string data,int opt)
    {
        // idRegistro       route                                            bloque
        // 100              titanic/plato2/superficie1/pista4/sector5           3

        switch (opt) {
            case 1:{
                //select
                Item <int> * item = DM->disk->btree->searchItemById(stoi(data));
                stringstream ss(item->route);
                int nroBloque,nroSecBloq;
                ss>>nroBloque;
                ss>>nroSecBloq;
                cout<<"\tBloque recuperado (id): "<<nroBloque<<endl;
                cout<<"\tInformacion del registro en disco: "<<DM->directorioBloques[nroBloque]->sectores[nroSecBloq]->route<<endl;

                Bloque *bloq = DM->directorioBloques[nroBloque];
                bloq->getInfoBloque();
                BM->insertPage(nroBloque,bloq);
                break;
            }
            case 2: {
                //insertar
                string freeSpaceDisk = getFreeSpace();
                stringstream ss(freeSpaceDisk);
                string bloq,bloqSector,lineSector;
                string dataInsert="";

                if(freeSpaceDisk.length() > 5){
                    std::istringstream iss(freeSpaceDisk);
                    iss >> bloq;
                    iss >> bloqSector;
                    iss >> lineSector;
                    dataInsert = bloqSector + "#" + lineSector + "#" + data;
                }
                else{
                    string lastSpace = getLastSpace();
                    std::istringstream iss(lastSpace);
                    iss >> bloq;
                    iss >> bloqSector;
                    iss >> lineSector;
                    dataInsert = bloqSector + "#" + lineSector + "#" + data;
                }

                //cout<<dataInsert<<endl;
                Bloque *block = DM->directorioBloques[stoi(bloq)];
                BM->insertPage(stoi(bloq),block);
                BM->modifyPage(stoi(bloq), 1, dataInsert);
                Item<int> item = {stoi(data.substr(0,5)),{bloq + " " + bloqSector + " " + lineSector}};
                DM->disk->btree->insert(item);
                cout<<"\tBloque recuperado (id): "<<bloq<<endl;
                cout<<"\tInformacion del registro en disco: "<<DM->directorioBloques[stoi(bloq)]->sectores[stoi(bloqSector)]->route<<endl;
                DM->disk->loadFreeSpace();

                DM->disk->updateFreeSpace(freeSpaceDisk);
                break;
            }
            case 3: {//eliminar
                Item <int> * item = DM->disk->btree->searchItemById(stoi(data));
                stringstream ss(item->route);
                int nroBloque,nroSecBloq;
                ss>>nroBloque;
                ss>>nroSecBloq;
                cout<<"\tBloque recuperado (id): "<<nroBloque<<endl;
                cout<<"\tInformacion obtenida del arbol: "<<item->route<<endl;
                cout<<"\tInformacion del registro en disco: "<<DM->directorioBloques[nroBloque]->sectores[nroSecBloq]->route<<endl;
                Bloque *bloq = DM->directorioBloques[nroBloque];
                BM->insertPage(nroBloque,bloq);
                BM->modifyPage(nroBloque, 2, data);
                DM->disk->updateAfterDeleteFreeSpace(item->route);
                break;
            }
        }
        cout<<"Estado Buffer Pool FLAGS: "<<endl;
        BM->printStateBuffer();
    }

};


#endif //BD2_DBMS_H
