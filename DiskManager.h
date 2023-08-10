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
    string mostrarInfoRegistroBloque(int nroBloque){
        Bloque *bloq = directorioBloques[nroBloque];
        string data = "";
        for(auto i: bloq->sectores){
            data += i->getUbicacionIds();
        }
        return data;
    }

    string mostrarInfoRegistroSolo(int block, int sectBlock, int lineSector)
    {
        string data = directorioBloques[block]->sectores[sectBlock]->route;
        ifstream fileOpen( data, ios::binary);
        string linea;
        getline(fileOpen, linea);
        int recordSize = fileOpen.tellg();

        fileOpen.seekg(ios::beg);


        fileOpen.seekg((recordSize)*(lineSector - 1), std::ios::beg);
        getline(fileOpen, linea);

        fileOpen.close();

        return data + " \n" + linea;
    }

    void adcionarRegistroEnBloque(string data, int idBloque)
    {


        for (auto a: directorioBloques[idBloque]->sectores)
        {
            if(a->memoriaDisponible > data.length()) {
                ofstream archivo(a->route, ios::app);
                archivo << data << endl;
                cout << "Registro insertado en bloque con espacio" << endl;
                cout << "\t" << a->route << endl;
                return;
            }
        }

        // caso de bloque lleno

        string modifyIndfo = disk->getFreeSpace();
        if(modifyIndfo.size()> 1)
        {
            vector<string> modify;
            string temp;
            stringstream is(modifyIndfo);
            while(is>>temp)
                modify.push_back(temp);

            string ruta =  disk->nameDisk+"/plato"+ modify[0]+"/superficie"+modify[1]+"/pista"+modify[2]+"/";
            ofstream file(ruta +"sector"+modify[3]+".txt",ios::app);
            file<<data<<endl;

        }
        else
        {
           string ruta =  disk->nameDisk+"/plato"+ to_string(disk->currentIndexes[0])+"/superficie"+to_string(disk->currentIndexes[1])+"/pista"+to_string(disk->currentIndexes[2])+"/";

            ofstream file(ruta+ "sector"+to_string(disk->currentIndexes[3])+".txt",ios::app);
            file<<data<<endl;
        }
    }

};

#endif //BD2_DISKMANAGER_H
