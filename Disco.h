//
// Created by leonf on 24/07/2023.
//
#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>
#include "sstream"
#include <string>
#include <cstring>

using namespace std;
namespace fs = std::filesystem;
#ifndef BD2_DISCO_H
#define BD2_DISCO_H

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
};

class Pista{
public:
    vector<Sector *> sectores;
    Pista(int numSectores){
        sectores.resize(numSectores,0);
    }
};

class Superficie{
public:
    vector<Pista *> pistas;
    Superficie(int numPistas){
        pistas.resize(numPistas,0);
    }
};

class Platos{
public:
    vector<Superficie *> superficies;

    Platos(int numSuperficies){
        superficies.resize(2,0);
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
    vector<Platos *> platos;
    Disco(string _tableName,int num_platos,int pista,int sector,int memoria,int _sectorBloque){
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

        ifstream infoTxt(nameTable+"/info.txt");
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

    void loadDisk(){

        for(int i=0;i<numPlatos;i++){
            platos[i] = new Platos(numPlatos);
            for(int j=0;j<numSuperficies;j++){
                platos[i]->superficies[j] = new Superficie(2);
                for(int k=0;k<numPista;k++){
                    platos[i]->superficies[j]->pistas[k] = new Pista(numPista);
                    for(int m=0;m<numSector;m++){
                        string route = nameDisk+"/plato"+ to_string(i)+"/superficie"+to_string(j)+"/pista"+to_string(k)+"/sector"+
                                                                                                                        to_string(m)+".txt";                        string routeB = "/pista"+to_string(k)+"/sector"+ to_string(m)+".txt";
                        platos[i]->superficies[j]->pistas[k]->sectores[m] = new Sector(route,memoriaSector);
                    }

                }

            }
        }
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
        std::vector<int> currentIndexes(4, 0);
        fs::path initialPath = fs::current_path();

        ifstream file("files/"+nameDisk+".txt");
        string linea;
        bool band = 1;
        while(getline(file,linea)){
            // Procesa la combinación actual de índices (currentPlato, currentSuperficie, currentPista, currentSector)
            fs::path route = nameDisk+"/plato"+ to_string(currentIndexes[0])+"/superficie"+to_string(currentIndexes[1])+"/pista"+to_string(currentIndexes[2])+"/";
            if (fs::exists(route)){
                fs::current_path(route);
                uintmax_t fileSize = fs::file_size("sector"+to_string(currentIndexes[3])+".txt");
                if(!(fileSize + linea.length() > memoriaSector)) {
                    ofstream file("sector"+to_string(currentIndexes[3])+".txt",ios::app);
                    file<<linea<<endl;
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
                        fs::current_path(initialPath);
                        fs::path routeA = nameDisk+"/plato"+ to_string(currentIndexes[0])+"/superficie"+to_string(currentIndexes[1])+"/pista"+to_string(currentIndexes[2])+"/";
                        fs::current_path(routeA);
                        ofstream file("sector"+to_string(currentIndexes[3])+".txt",ios::app);
                        file<<linea<<endl;
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
    }
};
#endif
