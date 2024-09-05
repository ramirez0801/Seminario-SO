//Emmanuel Ramirez Garcia 219293882 --- Se usa notacion Hungara


#include <bits/stdc++.h>

using namespace std;

//Estructuras
struct Proceso
{
    string sNombreProgramador, sOperacion;
    int nId, nTiempoEstimado;
};

struct Lote
{
    Proceso tPro[20];
    int nIdLot, nNumPro;
};

//Funciones
bool existeID(Lote[], int, int);
int operacion();

int main()
{
    int nLotes, nFaltantes, nIdProceso, nA, nB, nTiempo = 0;
    
    //Loop que no permite avanzar hasta que se elige un numero de lotes valido
    while(true)
    {
        cout<<"Escriba el numero de lotes a ejecutar: ";
        cin>>nLotes;
        if(nLotes < 0 || nLotes > 5)
            cout<<"El numero de lotes tiene que ser entre 1 y 5"<<endl;
        else
            break;
    }

    Lote tLo[nLotes];

    //For para los lotes
    for(int i = 0; i < nLotes; i++)
    {
        //int nProcces = 0;
        cout<<"************LOTE**************"<<endl;
        cout<<"Numero de procesos para el lote "<<i<<endl;
        cin>> tLo[i].nNumPro;
        
        //For para los procesos
        for(int j = 0; j < tLo[i].nNumPro; j++)
        {
            cout<<"****************PROCESO**************"<<endl;
            bool bBand = true;
            cout<<"\t\tProceso "<<j<<endl;
            cout<<"Nombre: ";
            cin>>tLo[i].tPro[j].sNombreProgramador;
            cout<<"Operacion: ";
            cin>>tLo[i].tPro[j].sOperacion;
            
            while(nTiempo == 0 || nTiempo < 0)
            {
                cout<<"Tiempo del proceso(Mayor a 0): ";
                cin>>nTiempo;
            }
            tLo[i].tPro[j].nTiempoEstimado = nTiempo;

            while(bBand)
            {
                cout<<"ID: ";
                cin>>nIdProceso;
                bBand = existeID(tLo,nIdProceso,i);
                if(bBand)
                    cout<<"ID ya existente"<<endl;
                else
                {
                   tLo[i].tPro[j].nId = nIdProceso;
                   cout<<"Guardado "<<tLo[i].tPro[j].nId<<endl;
                }
            }

        }
    }
}

bool existeID(Lote tLo[], int proceso, int nLotes)
{
    for(int k = 0; k <= nLotes; k++)
    {
        for(int l = 0; l < tLo[k].nNumPro; l++)
        {
            cout<<"Lote "<<tLo[k].tPro[l].nId<<endl;
            cout<<"NumPro "<<tLo[k].nNumPro<<"  nLote "<<nLotes<<endl;
            if(proceso == tLo[k].tPro[l].nId)
                return true;
        }

    }
    return false;
}