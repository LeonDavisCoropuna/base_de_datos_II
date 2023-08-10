#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

void addComas(){

    fstream archivo("files/data2.txt",ios::in|ios::app);
    ofstream archivo2("files/data2.txt");
    string linea;
    while(getline(archivo,linea)){
        linea += ",";
        archivo2<<linea<<endl;
    }
}

void generate_txt(int number, int *pesos, string file_original, string file_end)
{
    cout << "or: " << file_original << endl;
    cout << "end: " << file_end << endl;

    ifstream archivo("files/data2.txt");
    fstream archivo2("files/carData.txt", fstream::app);

    string header = "";
    for (int i = 0; i < number; i++)
    {
        for (int q = 0; q < pesos[i]; q++)
        {
            header += " ";
        }
    }

    char c;
    int capacidad = 2;
    int j = 0;
    char *arreglo = new char[capacidad];
    bool header_created = false;

    while (archivo.get(c))
    {
        if (c == '\n')
        {
            string word = "";
            int h = 0;
            int aux = 0;

            while (arreglo[h] != '\0')
            {
                if (arreglo[h] == ',' || arreglo[h] == '\n')
                {
                    string palabra = word;

                    while (palabra.length() != pesos[aux])
                    {
                        if(palabra.length() < pesos[aux]){
                            palabra += " ";
                        }
                        else{
                            palabra = palabra.substr(0,palabra.length()-1);
                        }
                    }

                    archivo2 << palabra;
                    aux++;
                    word = "";
                }
                else
                {
                    word += arreglo[h];
                }
                h++;
            }
            archivo2 << endl;
            capacidad = 2;
            j = 0;
            delete[] arreglo;
            arreglo = new char[capacidad];
        }
        else
        {
            arreglo[j] = c;
            arreglo[j + 1] = '\0';
            j++;
            capacidad++;
            arreglo = (char *)realloc(arreglo, capacidad * sizeof(char)); // redimensionamos el arreglo
        }
    }

    archivo.close();
    archivo2.close();
    cout << "\n\tArchivo " << file_end << ".txt creado con exito...!" << endl;
}


int main(){
    int pesos[15] =     {5,12	,23,	6,	10,	9	,10,	12,	12,	14,	13,	17,	17	,7	,15};
    generate_txt(0,pesos,"data2","carData");
    return 0;
}