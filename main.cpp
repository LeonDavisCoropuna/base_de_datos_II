#include <iostream>
#include <vector>
#include "Disco.h"
#include "DiskManager.h"
#include "DBMS.h"
#include "BufferManager.h"
using namespace std;
int main() {
    /*
    Disco *disco;

    int usar = 0;
    cout<<"Crear disco: 0"<<endl;
    cout<<"Usar disco: 1"<<endl;
    cout<<"Opt: ";cin>>usar;
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
    }
    disco->loadDisk();
*/

    Disco *disco = new Disco("titanic");
    disco->loadDisk();
    DiskManager *manager = new DiskManager(disco );
    manager->makingBlok();
    //manager->showDirectorio();

    //manager.makingBTree(3,4);
    BufferManager *bufem = new BufferManager(4);
    DBMS *db = new DBMS(manager,bufem);
    db->sql_Request(14,3);

    return 0;
}
