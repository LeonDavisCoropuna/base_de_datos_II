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
void opt3(DiskManager *disk)
{
    int nBloq;
    string newData;
    cout<<"Numero de bloque: ";cin>>nBloq;
    cin.ignore();
    cout<<"Nuevo dato: ";    getline(cin,newData);
    disk->adcionarRegistroEnBloque(newData,nBloq);
}

<<<<<<< HEAD
=======
//Eliminar registro
void opt4(DBMS * &db)
{
    int recordId;
    cout<<"Ingrese id del Registro a eliminar: ";cin>>recordId;

    Item<int>* data= manager->disk->btree->searchItemById(recordId);
    string values = data->route;
    std::stringstream ss(values); // Crea un stringstream con la cadena de entrada
    string idBlock, secBlock, lineSector;
    ss>>idBlock;
    ss>>secBlock;
    ss>>lineSector;

    db->BM->modifyPage(stoi(idBlock), 2, to_string(recordId));
    db->BM->deletePage(stoi(idBlock));
    cout<<"Registro eliminado con exito! "<<endl;

}



>>>>>>> a86531ff3f2dca486329a655b0b00e62aa7ea297
/*________________________________________________________________________________________________________________________________*/

//Preguntas Buffer Manger

/*________________________________________________________________________________________________________________________________*/

<<<<<<< HEAD
// Consultar Registro
void opt4(DiskManager *disk)
{
=======
// Consultar Registro - Proceso de llevar una página al buffer pool (10 ptos):
/*
>>>>>>> a86531ff3f2dca486329a655b0b00e62aa7ea297

1.1. Mostrar que el buffer pool está vacio - Mostrar capacidad libre y ocupado
1.2. Buscar la posición del registro usando la estructura de la tabla de índices - Mostrar en pantalla como realiza la ubicación del registro
1.3. Mostrar en que bloque, sector, pista se encuentra el registro.
1.4. Recuperar bloque: Mostrar la estructura e información del bloque
1.5. Mostrar que la página está en buffer pool - Mostrar capacidad libre y ocupado
1.6. Mostrar los flags de las páginas (Estado de la página)
 */

void opt5(DBMS *db)
{
    cout<<"Estado de buffer pool"<<endl;

    cout<<db->BM->getStateBufferPool()<<endl;

    int idRecord;
    cout<<"Id del registro a buscar: "; cin>>idRecord;
    db->sql_Request(to_string(idRecord),1);

    cout<<"\nRegistro encontrado finalizado!"<<endl;
}

// Consultar un rango de registro contiguos
void opt5(DiskManager *disk)
{

}

// Consultar un rango de registros no contiguos
void opt6(DiskManager *disk)
{

}

// Eliminar registro
<<<<<<< HEAD
void opt7(DiskManager *disk)
=======
/*
 4.1. Mostrar que el registro se encuentra en una página del buffer pool o no.
4.2. Buscar la posición del registro usando la estructura de la tabla de índices - Mostrar en pantalla como realiza la ubicación del registro
4.3. Mostrar en que bloque, sector, pista se encuentra el registro.
4.4. Recuperar bloque: Mostrar la estructura e información del bloque
4.5. Mostrar que la página está en buffer pool - Mostrar capacidad libre y ocupado
4.6. Mostrar los flags de las páginas (Estado de la página)
 */
void opt8(DiskManager *disk)
>>>>>>> a86531ff3f2dca486329a655b0b00e62aa7ea297
{

}


//Adicionar registro
void opt8(DiskManager *disk)
{

}

// Manejo de buffer pool
void opt9(DiskManager *disk)
{

}

//Estructura de la tabla de indices
void opt10(DiskManager *disk)
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

    opt1(manager);
    opt3(manager);
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