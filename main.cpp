#include <iostream>
#include <vector>
#include "Disco.h"
using namespace std;
int main() {
    //Crear disco desde 0 a partir de un file
    //Disco disco("titanic",4,5,10,2500,5);
    //disco.createDisk();

    //Usar un disco
    Disco disco("titanic");

    disco.loadDisk();

    return 0;
}
