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
    int tiempo_max;
    char operacion;
    int operandos[2];
    string resultado;
    string estado;

    Proceso(int id, int tiempo_max, char operacion, int op1, int op2): id(id), tiempo_max(tiempo_max), operacion(operacion) 
    {
        operandos[0] = op1;
        operandos[1] = op2;
        estado = "En espera";
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


void interrupcion(std::vector<Proceso>& procesos) {
    if (!procesos.empty()) {
        Proceso proceso = procesos.front();
        procesos.erase(procesos.begin());
        proceso.estado = "Interrumpido";
        procesos.push_back(proceso);
        std::cout << "Proceso " << proceso.id << " interrumpido y movido al final de la cola.\n";
    }
}

void error(vector<Proceso>& procesos) {
    if (!procesos.empty()) {
        Proceso proceso = procesos.front();
        procesos.erase(procesos.begin());
        proceso.estado = "Error";
        proceso.resultado = "Error";
        cout << "Proceso " << proceso.id << " terminado por error.\n";
    }
}

void pausa() {
    std::cout << "Ejecución pausada. Presiona 'C' para continuar.\n";
    while (true) {
        if (cin.get() == 'C' || cin.get() == 'c') {
            break;
        }
    }
}

void ejecutar_procesos(vector<Proceso>& procesos) {
    char comando;
    int seg;

    queue <Proceso> termi;
    for (auto& proceso : procesos) {
        proceso.estado = "En ejecución";
        cout << "Ejecutando proceso " << proceso.id << "...\n";
        this_thread::sleep_for(chrono::milliseconds(proceso.tiempo_max * 100));
        
        cout<<left<<setw(20)<<"Nombre"<<setw(20)<<"Operacion"<<setw(20)<<"Tiempo"<<setw(20)<<"ID"<<endl;
        seg = proceso.tiempo_max * 10;
        for (int i = 0; i <= 100; ++i) 
        {
            cout << "\rProgreso: [" << string(i, '=') << string(100 - i, ' ') << "] " << i << "%";
            cout.flush();
            Sleep(seg);
            if(kbhit())
            {
                cout << "Presiona 'I' para interrupción, 'E' para error, 'P' para pausa, 'C' para continuar: ";
                cin >> comando;
                comando = toupper(comando);
                if (comando == 'I') 
                {
                    interrupcion(procesos);
                } 
                else if (comando == 'E') 
                {
                    error(procesos);
                } 
                else if (comando == 'P') 
                {
                    pausa();
                } 
                else if (comando == 'C') 
                {
                    continue;
                }
            }
        }
        proceso.ejecutar();
        std::cout << "Proceso " << proceso.id << " terminado con resultado: " << proceso.resultado << "\n";
    }
}

// void mostrar(vector<Proceso>& procesos)
// {
//     char key;
//     int seg;

// }

int main() {
    int num_procesos;
    queue <Lote> lotes;
    Lote loteAct;
    cout << "Número de proceos: ";
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

    cout<<"Lotes "<<lotes.size();

    // while (!procesos.empty()) 
    // {
    //     ejecutar_procesos(procesos);
    // }

    return 0;
}