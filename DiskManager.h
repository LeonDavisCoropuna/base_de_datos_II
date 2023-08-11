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
        string values;
        int keyData = stoi(data.substr(0,5));
        bool temp = false;
        ifstream freeSpaceFile(disk->nameDisk+"/freeSpace.txt");
        string line;
        while(getline(freeSpaceFile,line)){
            cout<<line<<"--"<<stoi(line)<<endl;
            if(stoi(line) == idBloque) {
                temp = true;
                break;
            }
        }
        freeSpaceFile.close();
        if(temp){
            vector<int>ubicacion;
            int num;
            stringstream iss(line);
            while(iss>>num) ubicacion.push_back(num);
            cout<<"Bloque con espacio en: "<<line<<endl;
            cout<<"Con respecto al disco: "<<directorioBloques[idBloque]->sectores[ubicacion[1]]->route<<endl;
            fstream sector(directorioBloques[idBloque]->sectores[ubicacion[1]]->route,ios::binary|ios::in|ios::out);
            int sizeLine;
            string lineaSize;
            sector.seekg(0,ios::beg);
            getline(sector,lineaSize);
            sizeLine = sector.tellg();
            sector.close();

            fstream sectorWrite(directorioBloques[idBloque]->sectores[ubicacion[1]]->route,ios::binary|ios::in|ios::out);
            if (data.size() < sizeLine) {
                data += std::string(sizeLine - data.size() - 1, ' ');
            }
            data += "\n";
            sectorWrite.seekg((sizeLine)*(ubicacion[2]-1), std::ios::beg);
            sectorWrite.write(data.c_str(),sizeLine);
            sectorWrite.close();
            Item<int> newData = {keyData,to_string(ubicacion[0]) + " " +to_string(ubicacion[1])+" "+ to_string(ubicacion[2])};
            disk->btree->insert(newData);
            directorioBloques[ubicacion[0]]->sectores[ubicacion[1]]->setMemoryAfterDelete(sizeLine);
            disk->loadFreeSpace();
            disk->updateFreeSpace(newData.route);

        }else{
            string routeLastPace = disk->getLastSpace();
            stringstream iss(routeLastPace);
            vector<int>lastPlaceRoute;
            int number;
            while(iss>>number) lastPlaceRoute.push_back(number);
            cout<<"Bloque sin espacio, insertando al final: "<<endl;
            cout<<"Ubicacion del nuevo registro: "<<endl;
            cout<<"\tRespecto al bloque: "<<routeLastPace<<endl;
            cout<<"\tRespecto al disco: "<<directorioBloques[lastPlaceRoute[0]]->sectores[lastPlaceRoute[1]]->route<<endl;
            string ruta = directorioBloques[lastPlaceRoute[0]]->sectores[lastPlaceRoute[1]]->route;
            fstream file(ruta,ios::app);
            file<<data<<endl;
            int lineRecordLast=0;
            string line;
            file.close();
            ifstream file2(ruta);
            while(getline(file2,line)) lineRecordLast++;
            Item<int> newData = {keyData,to_string(lastPlaceRoute[0]) + " " +to_string(lastPlaceRoute[1])+" "+ to_string(lineRecordLast)};
            disk->btree->insert(newData);
            directorioBloques[lastPlaceRoute[0]]->sectores[lastPlaceRoute[1]]->setMemoryAfterDelete(data.length());
        }
    }
    void eliminarRegistroDisco(int idRegistroToDelete){
        Item<int> *item = disk->btree->searchItemById(idRegistroToDelete);
        vector <int> ubicacion;
        string route = (item->route);
        stringstream iss(route);
        int num;
        while(iss>>num) ubicacion.push_back(num);
        fstream sector(directorioBloques[ubicacion[0]]->sectores[ubicacion[1]]->route,ios::binary|ios::in|ios::out);
        int sizeLine;
        string lineaSize;
        sector.seekg(0,ios::beg);
        getline(sector,lineaSize);
        sizeLine = sector.tellg();
        sector.close();

        fstream sectorWrite(directorioBloques[ubicacion[0]]->sectores[ubicacion[1]]->route,ios::binary|ios::in|ios::out);
        string data = "";
        if (data.size() < sizeLine) {
            data += std::string(sizeLine - data.size() - 1, ' ');
        }
        data += "\n";
        sectorWrite.seekg((sizeLine)*(ubicacion[2]-1), std::ios::beg);
        sectorWrite.write(data.c_str(),sizeLine);
        sectorWrite.close();
        Item<int> newData = {ubicacion[0],to_string(ubicacion[0]) + " " +to_string(ubicacion[1])+" "+ to_string(ubicacion[2])};
        cout<<"\tRespecto al bloque: "<<item->route<<endl;
        cout<<"\tRespecto al disco: "<<directorioBloques[ubicacion[0]]->sectores[ubicacion[1]]->route<<endl;
        directorioBloques[ubicacion[0]]->sectores[ubicacion[1]]->setMemoryAfterDelete(sizeLine);
        disk->updateAfterDeleteFreeSpace(item->route);
        //disk->btree->insert(newData);
        //disk->btree->bpt_print();
    }

};

#endif //BD2_DISKMANAGER_H
