#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <conio.h>
#include <bits/stdc++.h>
#include <windows.h>

using namespace std;

class Proceso 
{
public:
    int id;
    int tiempo_max, tiempo_trans;
    char operacion;
    int operandos[2];
    string resultado;
    string estado;

    Proceso(int id, int tiempo_max, char operacion, int op1, int op2): id(id), tiempo_max(tiempo_max), operacion(operacion) 
    {
        operandos[0] = op1;
        operandos[1] = op2;
        estado = "En espera";
        tiempo_trans = 0;
    }

    void ejecutar() 
    {
        try {
            switch (operacion) 
            {
                case '+':
                    resultado = std::to_string(operandos[0] + operandos[1]);
                    break;
                case '-':
                    resultado = std::to_string(operandos[0] - operandos[1]);
                    break;
                case '*':
                    resultado = std::to_string(operandos[0] * operandos[1]);
                    break;
                case '/':
                    if (operandos[1] != 0)
                        resultado = std::to_string(operandos[0] / operandos[1]);
                    else
                        throw std::runtime_error("División por cero");
                    break;
                case '%':
                    resultado = std::to_string(operandos[0] % operandos[1]);
                    break;
                default:
                    throw std::runtime_error("Operación desconocida");
            }
            estado = "Terminado";
        } catch (const std::exception& e) {
            resultado = "Error";
            estado = "Error";
        }
    }
};

class Lote
{
    public:
        vector <Proceso> pro;
};

vector<Proceso> generar_procesos(int num_procesos) {
    vector<Proceso> procesos;
    char operaciones[] = {'+', '-', '*', '/', '%'};
    srand(time(0));
    for (int i = 0; i < num_procesos; ++i) {
        int id = i + 1;
        int tiempo_max = rand() % 16 + 5;
        char operacion = operaciones[rand() % 5];
        int op1 = rand() % 100 + 1;
        int op2 = rand() % 100 + 1;
        procesos.emplace_back(id, tiempo_max, operacion, op1, op2);
    }
    return procesos;
}

void interrupcion(vector <Proceso>& procesos) {
    if (!procesos.empty()) {
        cout<<"Tiempo transcurrido: "<<procesos[0].id<<endl;
        procesos[0].estado = "Interrumpido";
        procesos.push_back(procesos[0]);
        procesos.erase(procesos.begin());
        cout << "Proceso " << procesos[0].id << " interrumpido y movido al final de la cola.\n";
    }
}

void error(vector<Proceso>& procesos) {
    // if (!procesos.empty()) {
    //     Proceso proceso = procesos.front();
    //     procesos.erase(procesos.begin());
    //     proceso.estado = "Error";
    //     proceso.resultado = "Error";
    //     cout << "Proceso " << proceso.id << " terminado por error.\n";
    // }
    cout<<"Interrrr";
}

void pausa() {
    std::cout << "Ejecución pausada. Presiona 'C' para continuar.\n";
    while (true) {
        if (cin.get() == 'C' || cin.get() == 'c') {
            break;
        }
    }
}

void mostrar(vector<Proceso>& procesos, queue <Lote>& lotes)
{
    char key;
    int seg;

    vector <Proceso> termi;

    while(!lotes.empty())
    {
        system("cls");
        Lote lo = lotes.front();
        lotes.pop();
        int tam = lo.pro.size();
        cout<<"Tam: "<<tam<<endl;
        for(int m = 0; m < tam; m++)
        {
            //For que itera sobre los procesos
            cout<<left<<setw(15)<<"Numero"<<setw(15)<<"Tiempo"<<setw(15)<<"Tiempo transcurrido"<<"\t\t\t Lotes restantes: "<<lotes.size()<<endl;
            for(int i = 0; i < tam; i++)
            {
                cout<<left<<setw(15)<<lo.pro[i].id<<setw(15)<<lo.pro[i].tiempo_max<<setw(15)<<lo.pro[i].tiempo_trans<<endl;
                lo.pro[i].ejecutar();
                
            }
            cout<<"Terminados: "<<endl;
            cout<<left<<setw(15)<<"Numero"<<setw(15)<<"Operacion"<<setw(15)<<"Resultado"<<endl;
            for(int j = 0; j < termi.size(); j++)
            {
                cout<<setw(15)<<termi[j].id<<setw(15)<<termi[j].operacion<<setw(15)<<termi[j].resultado<<endl;
            }
            cout<<endl;
            seg = lo.pro[m].tiempo_max * 10;
            for (int i = 0; i <= 100; ++i) 
            {
                cout << "\rProgreso: [" << string(i, '=') << string(100 - i, ' ') << "] " << i << "%";
                cout.flush();
                Sleep(seg);
                lo.pro[m].tiempo_trans = seg/10;
                if(kbhit())
                {
                    cout << "Presiona 'I' para interrupción, 'E' para error, 'P' para pausa, 'C' para continuar: ";
                    cin >> key;
                    key = toupper(key);
                    if (key == 'I') 
                    {
                        
                        interrupcion(lo.pro);
                        break;
                    } 
                    else if (key == 'E') 
                    {
                        //error(lo.pro[i]);
                        cout<<"ERROR";
                    } 
                    else if (key == 'P') 
                    {
                        pausa();
                    } 
                    else if (key == 'C') 
                    {
                        continue;
                    }
                }
            }
            if (key != 'I') {
                termi.push_back(lo.pro[m]);
            }
        }
       
    }
    cout<<"Exito";
}

int main() {
    int num_procesos;
    queue <Lote> lotes;
    Lote loteAct;
    cout << "Numero de proceos: ";
    cin >> num_procesos;

    vector<Proceso> procesos = generar_procesos(num_procesos);
    for(int i = 0; i<num_procesos; i++)
    {
        if(loteAct.pro.size() == 5)
        {
            lotes.push(loteAct);
            loteAct.pro.clear();
        }
        loteAct.pro.push_back(procesos[i]);
    }

    if (!loteAct.pro.empty()) 
    {
        lotes.push(loteAct);
    }

    cout<<"Lotes "<<lotes.size()<<endl;
    mostrar(procesos, lotes);

    return 0;
}