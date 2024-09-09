//Emmanuel Ramirez Garcia 219293882 --- Se usa notacion Hungara


#include <bits/stdc++.h>
#include <iomanip>
#include <conio.h>
#include <windows.h>


using namespace std;

//Estructura
struct Proceso
{
    string sNombreProgramador, sOperacion;
    int nId, nTiempoEstimado, nResult, nDato1, nDato2;
};

struct Lote
{
    vector <Proceso> tPro;
    int nIdLot, nNumPro;
};


//Funciones
void operacion(Proceso&);
bool existeID(vector<int>, int);

int main()
{
    int nLotes = 0, nFaltantes, nIdProceso, nTiempo = 0, nProc, cont = 0;
    vector <int> ids;

    cout<<"Numero de procesos: ";
    cin>> nProc;

    Proceso tPro[nProc];
    queue <Lote> lotes;
    Lote loteAct;
    vector <Proceso> proTerminado;

    for(int j = 0; j < nProc; j++)
    {
        bool bBand = true;
        nTiempo = 0;
        cout<<"\tProceso "<<j+1<<endl;
        cout<<"Nombre: ";
        cin>>tPro[j].sNombreProgramador;
        operacion(tPro[j]);

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
            }
        }

        if(loteAct.tPro.size() == 5)
        {
            lotes.push(loteAct);
            loteAct.tPro.clear();
        }
        loteAct.tPro.push_back(tPro[j]);


    }

    if (!loteAct.tPro.empty()) 
    {
        lotes.push(loteAct);
    }

    while(!lotes.empty())
    {
        cout<<"\nLotes pendientes: "<<lotes.size()-1<<endl;
        Lote lote = lotes.front();
        lotes.pop();

        cout<<"\n****Lote en ejecucion****"<<endl;
        cout << left << setw(20) << "Nombre" << setw(20) << "Tiempo"<<endl;
        for(auto& pro : lote.tPro)
        {
            cout<<setw(20)<<pro.sNombreProgramador<< setw(20) <<pro.nTiempoEstimado<<endl;
        }
        
        cout << "Presione cualquier tecla para continuar... "<<endl;
        getch();
        //Ejecucion del proceso
        cout<<endl;
        cout<<left<<setw(20)<<"Nombre"<<setw(20)<<"Operacion"<<setw(20)<<"Tiempo"<<setw(20)<<"ID"<<endl;
        for(auto& pro : lote.tPro)
        {
            
            cout<<setw(20)<<pro.sNombreProgramador<<setw(20)<<pro.sOperacion<<setw(20)<<pro.nTiempoEstimado<<setw(20)<<pro.nId<<endl;
            cont += pro.nTiempoEstimado;
            for (int i = 0; i <= 100; ++i) 
            {
                cout << "\rProgreso: [" << string(i, '=') << string(100 - i, ' ') << "] " << i << "%";
                cout.flush();
                Sleep(pro.nTiempoEstimado);
            }
            proTerminado.push_back(pro);
            cout << endl;
            
        }

        cout<<"\n***Proceso Terminados***"<<endl;
        cout<<left<<setw(20)<<"Nombre"<<setw(20)<<"Operacion"<<setw(15)<<"Dato 1"<<setw(15)<<"Dato 2"<<setw(20)<<"Resultado"<<endl;
        for(Proceso& proT : proTerminado)
        {
            cout<<setw(20)<<proT.sNombreProgramador<<setw(20)<<proT.sOperacion<<setw(15)<<proT.nDato1<<setw(15)<<proT.nDato2<<setw(15)<<proT.nResult<<endl;
        }
        cout<<"Tiempo transcurrido: "<<cont<<endl;
        cout << "Presione cualquier tecla para continuar... "<<endl;
        getch();

        system("cls");
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

void operacion(Proceso &pro)
{
    bool bFlag = true, bOperaFlag = false;
    string opera;

    do{
        bOperaFlag = false;
        cout<<"Operacion: ";
        cin>>pro.sOperacion;

        cout<<"Primer numero: ";
        cin>>pro.nDato1;

        cout<<"Segundo numero: ";
        cin>>pro.nDato2;

        if(pro.sOperacion == "/" || pro.sOperacion == "%")
        {
            if(pro.nDato2 == 0)
                while(bFlag)
                {
                    cout<<"Operacion invalida, vuleve a introducir el segundo numero: ";
                    cin>>pro.nDato2;
                    if(pro.nDato2 != 0)
                        bFlag = false;
                }
        }

        if(pro.sOperacion == "/")
            pro.nResult = pro.nDato1 / pro.nDato2;
        else if(pro.sOperacion == "%")
            pro.nResult = pro.nDato1 % pro.nDato2;
        else if(pro.sOperacion == "*")
            pro.nResult = pro.nDato1 * pro.nDato2;
        else if(pro.sOperacion == "+")
            pro.nResult = pro.nDato1 + pro.nDato2;
        else if(pro.sOperacion == "-")
            pro.nResult = pro.nDato1 - pro.nDato2;
        else
        {
            cout<<"Operacion invalida"<<endl;
            bOperaFlag = true;
        }

    }while(bOperaFlag);

}