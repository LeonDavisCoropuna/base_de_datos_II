//
// Created by leonf on 25/07/2023.
//
#include "Disco.h"
#include "Bloque.h"
#include "BTree.h"
#ifndef BD2_DISKMANAGER_H
#define BD2_DISKMANAGER_H

class DiskManager{
public:
    vector <Bloque *> directorioBloques;
    Disco * disk;

    DiskManager(Disco *_disk){
        disk = _disk;
    }
    void makingBlok(){
        std::vector<int> maxValues = {disk->numPlatos, disk->numSuperficies, disk->numPista, disk->numSector};
        std::vector<int> currentIndexes(4, 0);
        int maxSector = disk->sectorBloque;
        int temp = 0;
        Bloque *bloq = new Bloque(disk);
        bool band = 1;
        while (true) {
            if(temp < maxSector ){
                bloq->sectores.push_back(disk->platos[currentIndexes[0]]->superficies[currentIndexes[1]]->pistas[currentIndexes[2]]->sectores[currentIndexes[3]]);
                temp++;
                band = 1;
            }
            else{
                directorioBloques.push_back(bloq);
                bloq = new Bloque(disk);
                temp = 0;

                bloq->sectores.push_back(disk->platos[currentIndexes[0]]->superficies[currentIndexes[1]]->pistas[currentIndexes[2]]->sectores[currentIndexes[3]]);
                temp++;
            }

            int carry = 1;
            for (int i = 3; i >= 0; --i) {
                currentIndexes[i] += carry;
                carry = currentIndexes[i] / maxValues[i];
                currentIndexes[i] %= maxValues[i];
            }
            if (carry == 1)
                break;
        }
        directorioBloques.push_back(bloq);
    }
    void showDirectorio(){
        for(auto i: directorioBloques){
            i->cargarData();
            cout<<i->data;
        }
    }

    BTree * makingBTree(int nKeys,int nChildren,int x){
        //BTree<int> btree(nKeys,nChildren);
        BTree *btree = new BTree(3);

        for(int i=0;i<directorioBloques.size();i++){
            btree->insert(i);
        }
        return btree;
    }

};

#endif //BD2_DISKMANAGER_H
