//Emmanuel Ramirez Garcia 219293882 --- Se usa notacion Hungara


#include <bits/stdc++.h>
#include <iomanip>


using namespace std;

//Estructura
struct Proceso
{
    string sNombreProgramador, sOperacion;
    int nId, nTiempoEstimado, nResult;
};

//Funciones
int operacion(string);
bool existeID(vector<int>, int);

int main()
{
    int nLotes = 0, nFaltantes, nIdProceso, nTiempo = 0, nProc, cont = 0;
    vector <int> ids;

    cout<<"Numero de procesos: ";
    cin>> nProc;

    Proceso tPro[nProc];
    
    for(int j = 0; j < nProc; j++)
    {
        bool bBand = true;
        nTiempo = 0;
        if((j+1) % 5 == 0)
            nLotes++;
        
        cout<<"\t\tProceso "<<j+1<<endl;
        cout<<"Nombre: ";
        cin>>tPro[j].sNombreProgramador;
        cout<<"Operacion: ";
        cin>>tPro[j].sOperacion;
        tPro[j].nResult = operacion(tPro[j].sOperacion);
        cout<<"Lotes "<<nLotes<<endl;

        while(nTiempo <= 0)
        {
            cout<<"Tiempo del proceso(Mayor a 0): ";
            cin>>nTiempo;
        }
        tPro[j].nTiempoEstimado = nTiempo;

        while(bBand)
        {
            cout<<"ID: ";
            cin>>nIdProceso;
            bBand = existeID(ids,nIdProceso);
            if(bBand)
            {
                cout<<"ID ya existe"<<endl;
            }
            else
            {
                tPro[j].nId = nIdProceso;
                ids.push_back(nIdProceso);
                cout<<"Guardado "<<tPro[j].nId<<endl;
            }
        }

    }


    int nRestantes = 0;
    for(int i = 0; i <= nLotes; i++)
    {
        if(nLotes != 0)
            nLotes -= 1;
        
        cout<<"Lotes pendientes: "<<nLotes<<endl;
        cout<<"Nombre"<<setw(20)<<"Tiempo estimado"<<endl;
        for(int j = nRestantes; j < nProc; j++ )
        {
            if((j+1) % 5 == 0)
            {
                nRestantes = j+1;
                cout<<"Restantes: "<<nRestantes<<endl;
                break;
            }
            cout<< setw(5) <<tPro[j].sNombreProgramador << setw(10) << tPro[j].nTiempoEstimado<<endl;
        }
    }

    cout<<"Exito";

    return 0;
}

bool existeID(vector<int>ids, int idPro)
{
    int tamano = ids.size();
    for(int i = 0; i < tamano; i++ )
    {
        if(idPro == ids[i])
            return true;
    }
    return false;
}

int operacion(string opera)
{
    int nA,nB, nSum = 0;
    bool bFlag = true, bOperaFlag = false;
    
    do{
        bOperaFlag = false;
        cout<<"Primer numero: ";
        cin>>nA;

        cout<<"Segundo numero: ";
        cin>>nB;

        if(opera == "/" || opera == "%")
        {
            if(nB == 0)
                while(bFlag)
                {
                    cout<<"Operacion invalida, vuleve a introducir el segundo numero: ";
                    cin>>nB;
                    if(nB != 0)
                        bFlag = false;
                }
        }

        if(opera == "/")
            nSum = nA/nB;
        else if(opera == "%")
            nSum = nA % nB;
        else if(opera == "*")
            nSum = nA * nB;
        else if(opera == "+")
            nSum = nA + nB;
        else if(opera == "-")
            nSum = nA - nB;
        else
        {
            cout<<"Operacion invalida"<<endl;
            bOperaFlag = true;
        }

    }while(bOperaFlag);

    return nSum;
}