#include <iostream>
#include <vector>
#include "Disco.h"
#include "DiskManager.h"
#include "DBMS.h"
#include "BufferManager.h"
#include "BTree.h"
using namespace std;

void opt1(DiskManager *disk){
    int nroBloque;
    cout<<"Mostrar en que plato, superficie, pista y sector están los registros del bloque"<<endl;

    cout<<"\tNumero de bloque: ";cin>>nroBloque;
    cout<<"\tBloque: "<<nroBloque<<endl;
    string idsInfo = disk->mostrarInfoRegistroBloque(nroBloque);
    cout<<idsInfo<<endl;
}

//Consular un registro
void opt2(DiskManager *manager)
{
    cout<<"Consultar un registro:"<<endl;
    int idRecord;
    cout<<"\tId del Registro: ";cin>>idRecord;
    Item<int>* data= manager->disk->btree->searchItemById(idRecord);
    string values = data->route;
    std::stringstream ss(values); // Crea un stringstream con la cadena de entrada
    string idBlock, secBlock, lineSector;
    ss>>idBlock;
    ss>>secBlock;
    ss>>lineSector;

    string info = manager->mostrarInfoRegistroSolo(stoi(idBlock), stoi(secBlock), stoi(lineSector));

    cout<<info<<endl;

}

//Adicionar registro
void opt3(string newRecord, DiskManager *manager)
{
    manager->directorioBloques;
}

//Eliminar registro
void opt4(DiskManager *disk)
{

}



/*________________________________________________________________________________________________________________________________*/

//Preguntas Buffer Manger

/*________________________________________________________________________________________________________________________________*/

// Consultar Registro
void opt5(DiskManager *disk)
{

}

// Consultar un rango de registro contiguos
void opt6(DiskManager *disk)
{

}

// Consultar un rango de registros no contiguos
void opt7(DiskManager *disk)
{

}

// Eliminar registro
void opt8(DiskManager *disk)
{

}


//Adicionar registro
void opt9(DiskManager *disk)
{

}

// Manejo de buffer pool
void opt10(DiskManager *disk)
{

}

//Estructura de la tabla de indices
void opt11(DiskManager *disk)
{

}
void optCreate(Disco *&disco){
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
        cout<<"Nombre de la tablas: ";getline(cin,tableName);
        int numPlatos,numPistas,numSectores,memoriaSector,sectorBloques;
        cout<<"N Platos?: ";cin>>numPlatos;
        cout<<"N Pistas?: ";cin>>numPistas;
        cout<<"N Sectores?: ";cin>>numSectores;
        cout<<"Memoria por sector?: ";cin>>memoriaSector;
        cout<<"N sectores por bloque?: ";cin>>sectorBloques;
        disco = new Disco(tableName,numPlatos,numPistas,numSectores,memoriaSector,sectorBloques);

        disco->createDisk();
    }

}


int main() {

    Disco *disco;
    optCreate(disco);
    disco->loadDisk();
    cout<<"FINN"<<endl;
    DiskManager *manager = new DiskManager(disco );
    manager->makingBlok();
    BufferManager *bufem = new BufferManager(4);
    DBMS *db = new DBMS(manager,bufem);

    //opt1(manager);
    opt2(manager);

    //manager->showDirectorio();
    //string idsInfo = manager->mostrarInfoRegistroBloque(nroBloque);
/*
    //disco = new Disco("prueba");
    disco->loadDisk();
    DiskManager *manager = new DiskManager(disco );
    manager->makingBlok();
    BufferManager *bufem = new BufferManager(4);
    DBMS *db = new DBMS(manager,bufem);
    int x;
    while(cin>>x && x < 60){
        string data;
        getline(cin,data);
        db->sql_Request("841 Aldito Martinez Tu causaaaa ",2);
        Item <int> *w = disco->btree->searchItemById(x);
        cout<<w->key<<" -> "<<w->route<<endl;
    }
    */
    /*
   Node <int> *nodde = disco->btree->searchItemByRange(44);
   for(int i=0;i<nodde->size;i++)
       cout<<nodde->item[i].key<<"-";
*/
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
    disco->endProgram();

    return 0;
}