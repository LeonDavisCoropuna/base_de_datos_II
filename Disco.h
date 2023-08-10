//
// Created by leonf on 24/07/2023.
//

#ifndef BD2_DISCO_H
#define BD2_DISCO_H
#include <list>
#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>
#include "sstream"
#include <string>
#include <cstring>
#include <queue>
#include "BTree.h"
using namespace std;
namespace fs = std::filesystem;
class Sector;
class Pista;
class Superficie;
class Platos;

class Sector{
public:
    string route;
    int memoriaDisponible;
    int capacidad;
    Sector(string _route,int _capacidad){
        route = _route;
        capacidad = _capacidad;
        setMemoriaDisponible();
    }
    void setMemoriaDisponible(){
        uintmax_t fileSize = fs::file_size(route);
        memoriaDisponible = capacidad - fileSize;
    }
    string getData(){
        ifstream file(route);
        string data,linea;
        while(getline(file,linea)) data += linea + '\n';
        return data;
    }
    string getRegistro(string key){
        key = "5";
        int sizeKey = stoi(key); //cambiar
        string data,linea;
        ifstream file(route);
        while(getline(file,linea)){
            data += linea.substr(0,sizeKey) + " ";
        }
        return data;
    }
    string getUbicacionIds(){
        //cout<<route<<endl;
        ifstream file(route);
        string data,linea;
        while(getline(file,linea)) data += linea.substr(0,5) +  route + '\n';
        return data;
    }
};

class Pista{
public:
    vector<Sector *> sectores;
    Pista(int numSectores){
        sectores.resize(numSectores,0);
    }
};

class Superficie {
public:
    vector<Pista *> pistas;
    Superficie(int numPistas) {
        pistas.resize(numPistas, nullptr);
    }
};

class Platos {
public:
    vector<Superficie *> superficies;
    Platos(int numSuperficies) {
        superficies.resize(numSuperficies, nullptr);
    }
};

class Disco{
public:
    string nameDisk;
    int numPlatos;
    int numPista;
    int numSector;
    int memoriaSector;
    int sectorBloque;
    const int numSuperficies = 2;
    queue <string> freeSpaceList;
    vector<Platos *> platos;
    BPlusTree <int>*btree;
    std::vector<int> currentIndexes;

    Disco(string _tableName,int num_platos,int pista,int sector,int memoria,int _sectorBloque){
        btree = new BPlusTree<int>(10);
        currentIndexes.resize(4,0);
        nameDisk = _tableName;
        numPlatos = num_platos;
        numPista = pista;
        numSector = sector;
        memoriaSector = memoria;
        sectorBloque = _sectorBloque;
        platos.resize(numPlatos,0);


    }
    Disco(string nameTable){
        nameDisk = nameTable;
        btree = new BPlusTree<int>(10);
        currentIndexes.resize(4,0);
        ifstream infoTxt(nameDisk+"/info.txt");
        ofstream archiveFreeSpace(nameDisk+"/freeSpace.txt");
        archiveFreeSpace.close();
        string linea;
        getline(infoTxt,linea);
        stringstream ss(linea);
        ss>>numPlatos;
        ss>>numPista;
        ss>>numSector;
        ss>>memoriaSector;
        ss>>sectorBloque;
        platos.resize(numPlatos,0);

    }
    void loadFreeSpace(){
        ifstream archiveFreeSpace(nameDisk+"/freeSpace.txt");
        string linea;
        while(getline(archiveFreeSpace,linea)){
            cout<<linea<<endl;
            freeSpaceList.push(linea);
        }
    }
    string getFreeSpace(){
        loadFreeSpace();
        string top = freeSpaceList.front();

       /* while (true)
        {

            // Usando la función find()
            size_t posicion = top.find("LAST PLACE:");
            if (posicion != std::string::npos) {
                std::cout << "La subcadena '" << subcadena << "' se encuentra en la posición " << posicion << std::endl;
            } else {
                std::cout << "La subcadena no se encontró" << std::endl;
            }
        }
        */

        freeSpaceList.pop();
        return top;
    }
    void updateFreeSpace(){
        ofstream free(nameDisk+"/freeSpace.txt");
        while (!freeSpaceList.empty()) {
            free<< freeSpaceList.front()<<endl; // Acceder al elemento frontal de la cola
            freeSpaceList.pop(); // Eliminar el elemento frontal de la cola
        }
    }
    void makeBPlusTree(string id,int lineaSector,int secBloc,int bloque){

        try{
            Item<int> item = {stoi(id),to_string(bloque)+" "+ to_string(secBloc) + " " + to_string(lineaSector)};
            btree->insert(item);
            //btree->insert(stoi(id)," " + to_string(bloque)+" "+ to_string(secBloc) + " " + to_string(lineaSector));
        } catch (const std::invalid_argument& e){
            std::cerr << "Error de argumento: " << e.what() << std::endl;
        }
    };
    void loadBPlusTree(){
        std::ifstream  btreeFile(nameDisk+"/bPlusTree.txt");
        string linea;
        int bloque;
        string numBloque,secBloque,lineSector;
        while(std::getline(btreeFile,linea)){
            if(linea.length()>0){
                stringstream ss(linea);
                ss>>bloque;
                ss>>numBloque;
                ss>>secBloque;
                ss>>lineSector;
                Item<int> item = {bloque,numBloque+" " +secBloque+" " +lineSector};
                btree->insert(item);
            }
        }
    }
    void loadDisk() {
        //cout << "nnn: " << numPlatos << " " << numSuperficies << " " << numPista << " " << numSector << endl;
        for (int i = 0; i < numPlatos; i++) {
            if (!platos[i])
                platos[i] = new Platos(numSuperficies);
            for (int j = 0; j < numSuperficies; j++) {
                if (!platos[i]->superficies[j])
                    platos[i]->superficies[j] = new Superficie(numPista);

                for (int k = 0; k < numPista; k++) {
                    if (!platos[i]->superficies[j]->pistas[k])
                        platos[i]->superficies[j]->pistas[k] = new Pista(numSector);

                    for (int m = 0; m < numSector; m++) {
                        string route = nameDisk + "/plato" + std::to_string(i) + "/superficie" + std::to_string(j);
                        string routeb = "/pista" + std::to_string(k) + "/sector" + std::to_string(m) + ".txt";
                        string r = route + routeb;

                        if (!platos[i]->superficies[j]->pistas[k]->sectores[m])
                        {
                                platos[i]->superficies[j]->pistas[k]->sectores[m] = new Sector(r, memoriaSector);

                        }


                        route.clear();
                        routeb.clear();
                    }
                }
            }
        }
        ofstream archiveFreeSpace(nameDisk+"/freeSpace.txt");
        archiveFreeSpace.close();
        loadBPlusTree();
    }
    void createDisk(){
        // Utilizando la función del sistema para crear la carpeta en Windows
        string command = "mkdir " + nameDisk;
        if (system(command.c_str()) == 0) {
            cout << "Carpeta creada exitosamente.\n";
        } else {
            cout << "Error al crear la carpeta.\n";
        }
        ofstream fileInfo(nameDisk+"/info.txt");
        fileInfo<<numPlatos<<" "<<numPista<<" "<<numSector<<" "<<memoriaSector<<" "<<sectorBloque<<endl;
        fileInfo.close();
        createRoutes();
    }
    void createRoutes(){

        fs::path currentPath = fs::current_path();
        fs::path route = currentPath / nameDisk;

        for(int i=0;i<numPlatos;i++){
            string carpetaPlato = "plato" + to_string(i);
            fs::create_directory(route/carpetaPlato);

            for(int j=0;j<numSuperficies;j++){
                fs::path route2 = route / carpetaPlato;
                string carpetaSuperficie = "superficie"+to_string(j);
                fs::create_directory(route2/carpetaSuperficie);
                for(int k=0;k<numPista;k++){

                    fs::path route3 = route2 / carpetaSuperficie;
                    string carpetaPista = "pista"+to_string(k);
                    fs::create_directory(route3/carpetaPista);

                    for(int m=0;m<numSector;m++){
                        fs::path route4 = route3 / carpetaPista;
                        string sectorName = "sector"+to_string(m)+".txt";
                        fs::path newFilePath = route4 / sectorName;
                        std::ofstream outputFile(newFilePath);
                    }
                }
            }
        }
        createArchive();
    }
    void createArchive(){

        std::vector<int> maxValues = {numPlatos, numSuperficies, numPista, numSector};

        fs::path initialPath = fs::current_path();

        ifstream file("files/"+nameDisk+".txt");
        string linea;
        bool band = 1;
        int idBloq = 0;
        int temp = 1;
        int lineaFile = 0;
        while(getline(file,linea)){
            // Procesa la combinación actual de índices (currentPlato, currentSuperficie, currentPista, currentSector)
            fs::path route = nameDisk+"/plato"+ to_string(currentIndexes[0])+"/superficie"+to_string(currentIndexes[1])+"/pista"+to_string(currentIndexes[2])+"/";
            if (fs::exists(route)){
                fs::current_path(route);
                uintmax_t fileSize = fs::file_size("sector"+to_string(currentIndexes[3])+".txt");
                if(!(fileSize + linea.length() > memoriaSector)) {
                    ofstream file("sector"+to_string(currentIndexes[3])+".txt",ios::app);
                    file<<linea<<endl;
                    lineaFile++;
                    makeBPlusTree(linea.substr(0,5),lineaFile,(temp-1)%sectorBloque,idBloq);
                    band = 1;
                }
                else{
                    int carry = 1;
                    for (int i = 3; i >= 0; --i) {
                        currentIndexes[i] += carry;
                        carry = currentIndexes[i] / maxValues[i];
                        currentIndexes[i] %= maxValues[i];
                    }
                    // Si el acarreo es 1 en el primer índice, significa que hemos terminado de iterar
                    if (carry == 1){
                        cout<<"Disco lleno";
                        break;
                    }

                    if(band){
                        if(temp%sectorBloque == 0) { idBloq++;}
                        temp++;

                        fs::current_path(initialPath);
                        fs::path routeA = nameDisk+"/plato"+ to_string(currentIndexes[0])+"/superficie"+to_string(currentIndexes[1])+"/pista"+to_string(currentIndexes[2])+"/";
                        fs::current_path(routeA);
                        ofstream file("sector"+to_string(currentIndexes[3])+".txt",ios::app);
                        file<<linea<<endl;
                        lineaFile = 1;
                        makeBPlusTree(linea.substr(0,5),lineaFile,(temp-1)%sectorBloque,idBloq);
                        band = 0;
                    }
                }
            }
            else{
                cout<<"Disco lleno"<<endl;
                return;
            }
            fs::current_path(initialPath);
        }

        //ofstream archivo (nameDisk+"freeSpace.txt");
        //archivo<<"LAST PLACE: "<<currentIndexes[0]<<" "<<" "<<currentIndexes[1]<<" "<<currentIndexes[2]<<" "<<currentIndexes[3]<<endl;


    }
    void endProgram(){
        ofstream file(nameDisk+"/bPlusTree.txt");
        btree->bpt_print(file);
        file.close();

        ofstream archivo (nameDisk+"/lastPlace.txt");
        for (auto a: currentIndexes)
        {
            archivo<<a<<" ";
        }
        archivo<<endl;

        //btree->bpt_print();

    }


};

#endif
