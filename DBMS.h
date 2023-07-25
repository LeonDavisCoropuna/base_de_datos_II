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

class DBMS{
public:
    BufferManager *BM;
    DiskManager * DM;
    DBMS(DiskManager *dm, BufferManager* bm){
        DM = dm;
        BM = bm;
    }
    void sql_Request(int recordNum)
    {

        string record;
        BTree *btree = DM->makingBTree(3,4,recordNum);
        btree->search(recordNum);
        cout<<btree->root->keys[0];
        /*
        bool band = false;
        for (int i = 0; i < DM->directorioBloques.size() -1 ; i++)
        {
            if(DM->nPages[i])
            {
                if(BM->addPage(i, recordNum, DM->nPages[i]->ptrPosition, DM->disk->getNameTable(), DM))
                {
                    band=false;
                    break;
                }else
                {
                    band =true;
                }
            }else
            {
                band = true;
                break;
            }
            //BM->unpinPage(i);
        }
        if(band)
        {
            cout<<"Registro no encontrado en el DISCO"<<endl;
        }
         */
    }
};


#endif //BD2_DBMS_H
