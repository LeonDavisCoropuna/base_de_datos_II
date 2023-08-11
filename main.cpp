#include <iostream>
#include <vector>
#include "Disco.h"
#include "DiskManager.h"
#include "DBMS.h"
#include "BufferManager.h"
#include "BTree.h"
using namespace std;


void opt1(DiskManager *disk){ //mostrar registro por bloque grupo
    int nroBloque;
    cout<<"Mostrar en que plato, superficie, pista y sector están los registros del bloque"<<endl;

    cout<<"\tNumero de bloque: ";cin>>nroBloque;
    cout<<"\tBloque: "<<nroBloque<<endl;
    string idsInfo = disk->mostrarInfoRegistroBloque(nroBloque);
    cout<<idsInfo<<endl;
}

void opt2(DiskManager *manager) //Consular un registro unico
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
//Eliminar registro
void opt3_1(DiskManager *disk){
    int idRegistro;
    string newData;
    cout<<"Id registro a eliminar: ";cin>>idRegistro;
    disk->eliminarRegistroDisco(idRegistro);
}

/*________________________________________________________________________________________________________________________________*/

//Preguntas Buffer Manger

/*________________________________________________________________________________________________________________________________*/
void releasePageBuffer(DBMS *db){
    int opt,idPage;
    db->BM->printStateBuffer();
    cout<<"\tPagina a liberar (-1 omitir): ";cin>>idPage;
    if(idPage == -1) return;
    cout<<"\t1. Hacer 1 unpinned"<<endl;
    cout<<"\t2. Quitar pagina del buffer"<<endl;
    cout<<"\tOpt: ";cin>>opt;
    if(opt == 1){
        db->BM->realeasePage(idPage);
    } else if(opt == 2){
        db->BM->deletePage(idPage);
    }
    return;
}
// Consultar Registro

void opt5(DBMS *db) { //state buffer pool
    cout << "Estado de buffer pool" << endl;
    cout << db->BM->getStateBufferPool() << endl;
    int idRecord;
    cout << "Id del registro a buscar: ";
    cin >> idRecord;
    db->sql_Request(to_string(idRecord), 1);
    releasePageBuffer(db);
}

// Consultar un rango de registros no contiguos
void opt6(DBMS *db) {
    int a,b;
    cout<<"Inicio rango: ";cin>>a;
    cout<<"Fin rango: ";cin>>b;
    Node<int> *range = db->DM->disk->btree->searchItemByRange(a);
    for(int i=0;i<range->size;i++){
        db->sql_Request(to_string(range->item[i].key),1);
    }
    releasePageBuffer(db);
}
// Consultar un rango de registros no contiguos
void opt8(DBMS *db) {
    int a,b;
    cout<<"Inicio rango: ";cin>>a;
    cout<<"Fin rango: ";cin>>b;
    Node<int> *range = db->DM->disk->btree->searchItemByRange(a);
    for(int i=0;i<range->size;i++){
        db->sql_Request(to_string(range->item[i].key),1);
    }
    releasePageBuffer(db);
}
/*
 *
4.1. Mostrar que el registro se encuentra en una página del buffer pool o no.
4.2. Buscar la posición del registro usando la estructura de la tabla de índices - Mostrar en pantalla como realiza la ubicación del registro
4.3. Mostrar en que bloque, sector, pista se encuentra el registro.
4.4. Recuperar bloque: Mostrar la estructura e información del bloque
4.5. Mostrar que la página está en buffer pool - Mostrar capacidad libre y ocupado
4.6. Mostrar los flags de las páginas (Estado de la página)
 */

//eliminar registro buffer
void opt7(DBMS *db)
{
    cout<<"Estado de buffer pool"<<endl;
    cout<<db->BM->getStateBufferPool()<<endl;
    int idRecord;
    cout<<"Id del registro a eliminar: "; cin>>idRecord;
    db->sql_Request(to_string(idRecord),3);
    cout << "Estado de buffer pool" << endl;
    releasePageBuffer(db);
}

//Adicionar registro
void opt8_1(DBMS *db) {
    cout<<"Estado de buffer pool"<<endl;
    cout<<db->BM->getStateBufferPool()<<endl;
    string record;
    cout<<"Nuevo registro a insertar: ";getline(cin,record);
    db->sql_Request(record,2);
    releasePageBuffer(db);
}

void opt9(DBMS *db) { //mostrar paginas y contenido
    cout<<"Paginas en el buffer y su contenido: "<<endl;
    db->BM->showContenido();
}

//Estructura de la tabla de indices
    void opt10(DiskManager *disk) {

    }
    void optCreate(Disco *&disco) {
        int usar = 0;
        cout << "Crear disco: 0" << endl;
        cout << "Usar disco: 1" << endl;
        cout << "Opt: ";
        cin >> usar;
        cin.ignore();
        string tableName = "";
        if (usar) {
            cout << "Nombre de la tabla: ";
            getline(cin, tableName);
            disco = new Disco(tableName);
        } else {
            cout << "Nombre de la tablas: ";
            getline(cin, tableName);
            int numPlatos, numPistas, numSectores, memoriaSector, sectorBloques;
            cout << "N Platos?: ";
            cin >> numPlatos;
            cout << "N Pistas?: ";
            cin >> numPistas;
            cout << "N Sectores?: ";
            cin >> numSectores;
            cout << "Memoria por sector?: ";
            cin >> memoriaSector;
            cout << "N sectores por bloque?: ";
            cin >> sectorBloques;
            disco = new Disco(tableName, numPlatos, numPistas, numSectores, memoriaSector, sectorBloques);
            disco->createDisk();
        }
    }


int main() {

    Disco *disco;
    optCreate(disco);
    disco->loadDisk();

    DiskManager *manager = new DiskManager(disco );
    manager->makingBlok();
    BufferManager *bufem = new BufferManager(4);
    DBMS *db = new DBMS(manager,bufem);
/*
    db->sql_Request("36544",2);
    db->sql_Request("98745",2);
    db->sql_Request("33333",2);
    releasePageBuffer(db);
    releasePageBuffer(db);
    releasePageBuffer(db);


    //opt7(db);
    //opt7(db);

    disco->endProgram();
*/
    manager->disk->removeBPlusTree(20000);
    return 0;
}