#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <conio.h>
#include <iomanip>
#include <queue>
#include <vector>
#include <windows.h>

using namespace std;

class Proceso {
public:
    int id;
    int tiempo_max, tiempo_trans;
    char operacion;
    int operandos[2];
    string resultado;
    string estado;

    Proceso(int id, int tiempo_max, char operacion, int op1, int op2): id(id), tiempo_max(tiempo_max), operacion(operacion) {
        operandos[0] = op1;
        operandos[1] = op2;
        estado = "En espera";
        tiempo_trans = 0;
    }

    void ejecutar() {
        try {
            switch (operacion) {
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

class Lote {
public:
    vector<Proceso> pro;
};

vector<Proceso> generar_procesos(int num_procesos) {
    vector<Proceso> procesos;
    char operaciones[] = {'+', '-', '*', '/', '%'};
    srand(static_cast<unsigned int>(time(0)));
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

void pausa() {
    std::cout << "Ejecución pausada. Presiona 'C' para continuar.\n";
    while (true) {
        if (cin.get() == 'C' || cin.get() == 'c') 
        {
            break;
        }
    }
}

void mostrar(queue<Lote>& lotes) {
    char key;
    int aux = 0, seg, global = 0;
    float porcentaje;
    vector <Proceso> termi;

    while (!lotes.empty()) 
    {
        system("cls");
        Lote lo = lotes.front();
        lotes.pop();

        vector<Proceso> procesos_actuales = lo.pro; // Copia de los procesos

        // Ejecutar todos los procesos en el lote actual
        for (size_t m = 0; m < procesos_actuales.size(); m++) 
        {
            int tam = procesos_actuales.size();
            Proceso& proceso = procesos_actuales[m]; // Referencia al proceso
            proceso.ejecutar(); // Ejecutar el proceso
            cout<<endl;
            //For que itera sobre los procesos
            cout<<left<<setw(15)<<"Numero"<<setw(15)<<"Tiempo"<<setw(15)<<"Tiempo transcurrido"<<"\t\t\t Lotes restantes: "<<lotes.size()<<endl;
            for(int i = 0; i < tam; i++)
            {
                cout<<left<<setw(15)<<procesos_actuales[i].id<<setw(15)<<procesos_actuales[i].tiempo_max<<setw(15)<<procesos_actuales[i].tiempo_trans<<setw(2)<<"%"<<endl;      
            }
            cout<<"\nTerminados: "<<endl;
            cout<<left<<setw(15)<<"Numero"<<setw(15)<<"Operacion"<<setw(15)<<"Resultado"<<"\t\t\tContador global: "<<global/100<<endl;
            for(int j = 0; j < termi.size(); j++)
            {
                cout<<setw(15)<<termi[j].id<<setw(15)<<termi[j].operacion<<setw(15)<<termi[j].resultado<<endl;
            }
            cout<<endl;

            cout << "Ejecutando Proceso " << proceso.id << endl;

            // Simulando tiempo de ejecución
            if(proceso.tiempo_trans != 0)
                aux = proceso.tiempo_trans; 
            
            seg = proceso.tiempo_max * 10;// Ajustar el tiempo para Sleep
            //global += proceso.tiempo_max;
            cout<<"seg"<<seg<<endl;
            for (int i = aux; i <= 100; ++i) 
            {
                key = '\0';
                cout << "\rProgreso: [" << string(i, '=') << string(100 - i, ' ') << "] " << i << "%";
                cout.flush();
                Sleep(seg); // Simular tiempo de ejecución
                global += seg/10;
                
                if (kbhit()) 
                {
                    cout << "\nPresiona 'I' para interrupcion, 'P' para pausa, 'E' para terminar con error: ";
                    cin >> key;
                    key = toupper(key);
                    if (key == 'I') 
                    {
                        cout << "Proceso " << proceso.id << " interrumpido.\n";
                        proceso.tiempo_trans = i;
                        procesos_actuales.push_back(proceso);// Mover el proceso interrumpido al final
                        procesos_actuales.erase(procesos_actuales.begin() + m);
                        m--; // Ajustar índice después de eliminar
                        break; // Salir para volver a ejecutar el lote
                    } 
                    else if (key == 'P') 
                    {
                        pausa();
                    }
                    else if(key == 'E')
                    {
                        proceso.resultado = "Error";
                        cout<< "Proceso "<< proceso.id << " terminado con error.\n";
                        break;
                    }
                }
            }
            if (key != 'I') 
            {
                termi.push_back(proceso);
                procesos_actuales.erase(procesos_actuales.begin());
                m--;
            }
            //system("cls");
            
        }

            cout << "\nLote completado con los procesos restantes.\n";
            cout<<"Terminados: "<<endl;
            cout<<left<<setw(15)<<"Numero"<<setw(15)<<"Operacion"<<setw(15)<<"Resultado"<<"\t\t\tContador global: "<<global/100<<endl;
            for(int j = 0; j < termi.size(); j++)
            {
                cout<<setw(15)<<termi[j].id<<setw(15)<<termi[j].operacion<<setw(15)<<termi[j].resultado<<endl;
            }
            cout<<endl;
        
    }
    cout << "Ejecutado con exito" << endl;
    getch();
    
}

int main() {
    int num_procesos;
    queue<Lote> lotes;
    Lote loteAct;

    cout << "Numero de procesos: ";
    cin >> num_procesos;

    vector<Proceso> procesos = generar_procesos(num_procesos);
    for (int i = 0; i < num_procesos; i++) {
        if (loteAct.pro.size() == 5) {
            lotes.push(loteAct);
            loteAct.pro.clear();
        }
        loteAct.pro.push_back(procesos[i]);
    }

    if (!loteAct.pro.empty()) {
        lotes.push(loteAct);
    }

    cout << "Lotes: " << lotes.size() << endl;
    mostrar(lotes); // Pasar la cola de lotes a la función

    return 0;
}