#include <iostream>
#include <vector>
#include "Disco.h"
#include "DiskManager.h"
#include "DBMS.h"
#include "BufferManager.h"
#include "BTree.h"
using namespace std;
int main() {

    Disco *disco;
/*
    int usar = 0;
    cout<<"Crear disco: 0"<<endl;
    cout<<"Usar disco: 1"<<endl;
    cout<<"Opt: ";cin>>usar;
    cin.ignore();
    string tableName = "";
    if(usar){
        cout<<"Nombre de la tabla: ";getline(cin,tableName);
        disco = new Disco(tableName);
    }
    else{
        cout<<"Nombre de la tabla: ";getline(cin,tableName);
        int numPlatos,numPistas,numSectores,memoriaSector,sectorBloques;
        cout<<"N Platos?: ";cin>>numPlatos;
        cout<<"N Pistas?: ";cin>>numPistas;
        cout<<"N Sectores?: ";cin>>numSectores;
        cout<<"Memoria por sector?: ";cin>>memoriaSector;
        cout<<"N sectores por bloque?: ";cin>>sectorBloques;
        disco = new Disco(tableName,numPlatos,numPistas,numSectores,memoriaSector,sectorBloques);

        disco->createDisk();
    }
    */
    disco = new Disco("titanic",5,5,10,3000,5);
    disco->createDisk();
    disco->loadDisk();
    int searchKey = 891;
    Node * find = disco->btree->search(searchKey);
    if(find){
        for(auto i : find->keys){
            if (find->keys[i] == searchKey){
                std::string value = disco->btree->data[find->keys[i]];
                std::cout << "Clave: " << find->keys[i] << ", Valor: " << value << std::endl;
            }
        }
    }
    disco->generateBPlusTreeFile();
/*
    Disco *disco = new Disco("titanic");
    disco->loadDisk();
    DiskManager *manager = new DiskManager(disco );
    manager->makingBlok();
    //manager->showDirectorio();

    //manager.makingBTree(3,4);
    BufferManager *bufem = new BufferManager(4);
    DBMS *db = new DBMS(manager,bufem);
    // 1 select, 2 insert, 3 delete

    db->sql_Request("841",3);

    db->sql_Request("133",3);
    db->sql_Request("174",3);
    db->sql_Request("132",3);
    db->sql_Request("132",3);
*/
    //db->sql_Request("841 Aldito Martinez Tu causaaaa ",2);

/*
    BPlusTree bTree(10);

    const int numRandoms = 1000;
    for (int i = 0; i < numRandoms; ++i) {
        bTree.insert(i, "Hola "+std::to_string(i));
    }
    int searchKey = 513 ;
    Node * find = bTree.search(searchKey);
    if(find){
        for(auto i : find->keys){
            if (find->keys[i] == searchKey){
                std::string value = bTree.data[find->keys[i]];
                std::cout << "Clave: " << find->keys[i] << ", Valor: " << value << std::endl;
            }
        }
    }
    bTree.print();
*/

    return 0;
}
