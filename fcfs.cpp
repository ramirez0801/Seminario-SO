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

class Proceso { //Clase del proceso
public:
    int id;
    int tiempo_max, tiempo_trans, porcentaje, tLlegada, tSalida, tRtrn, tResp, tEspera, tServicio, contBloq;
    char operacion;
    int operandos[2];
    string resultado;
    string estado;

    Proceso(int id, int tiempo_max, char operacion, int op1, int op2) //Inicializacion
        : id(id), tiempo_max(tiempo_max), operacion(operacion) {
        operandos[0] = op1;
        operandos[1] = op2;
        estado = "Nuevo"; // Estado inicial
        tiempo_trans = 0;
        porcentaje = 0;
        tLlegada = 0;
        tSalida = 0;
        tRtrn = 0;
        tResp = 0;
        tEspera = 0;
        tServicio = 0;
        contBloq = 0;
    }

    void ejecutar() {
        estado = "Ejecución"; // Cambia el estado a ejecución al iniciar
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
    std::cout << "\nEjecucion pausada. Presiona 'C' para continuar.\n";
    while (true) {
        if (cin.get() == 'C' || cin.get() == 'c') {
            break;
        }
    }
}

void mostrar(vector<Proceso>& procesos) {
    char key;
    int aux, seg, global = 0, transcurrido = 0, r = 0;
    vector<Proceso> termi;
    vector<Proceso> bloqueado;
    queue<Proceso> memoria;
    while (!procesos.empty() || !memoria.empty() || !bloqueado.empty())
    {
        //system("cls"); 
        
        if(!procesos.empty() && memoria.size() + bloqueado.size() != 5)
        {
            r = 0;
            while(r < procesos.size()) //Mantem: Revisar r
            {
                procesos[r].estado = "Listo";
                procesos[r].tLlegada = global; //Tiempo llegada***
                memoria.push(procesos[r]);
                
                procesos.erase(procesos.begin() + r);
                
                if(memoria.size() == 5)
                {
                    //cout<<"Memoria llena\n";
                    break;
                }
            }
        }

        aux = 0;
        transcurrido = 0;

        Proceso proceso = memoria.front();  // Referencia al proceso
        memoria.pop();

        proceso.ejecutar();  // Ejecutar el proceso

        if (proceso.tiempo_trans != 0)
            aux = proceso.tiempo_trans;

        seg = proceso.tiempo_max * 10;  // Ajustar el tiempo para Sleep
        proceso.tResp = global- proceso.tLlegada; //Tiempo respuesta***
        // Simulando tiempo de ejecución
        for (int i = aux; i <= proceso.tiempo_max; ++i) 
        {
            key = '\0';
            cout << left << setw(15) << "Numero" << setw(15) << "Tiempo"<< setw(15) << "Estado"<< endl;
            queue<Proceso> mem_copy = memoria;
            while(!mem_copy.empty())
            {
                cout << left << setw(15) << mem_copy.front().id<< setw(15) << mem_copy.front().tiempo_max<< setw(15) << mem_copy.front().estado << endl;
                mem_copy.pop();
            }

            cout << "\nTerminados: " << endl;
            cout << left << setw(15) << "Numero" << setw(15) << "Operacion"<< setw(15) << "Resultado" << "\t\t\tContador global: " << global << endl;
            for (int j = 0; j < termi.size(); j++) 
            {
                cout << setw(15) << termi[j].id << setw(15) << termi[j].operacion<< setw(15) << termi[j].resultado << endl;
            }
            cout << endl;
            cout<<"Proceso en ejecucion: \n";
            cout << left << setw(15) << "Numero" << setw(15) << "Operacion"<< endl;
            cout << left << setw(15) << proceso.id << setw(15) << proceso.operacion << endl;

            cout<<"Tiempo transcurrido: "<<proceso.tiempo_trans<<endl;
            cout<<"Tiempo: "<<i;
            
            cout<<"\n\nBloqueados: \n";
            cout<<left<< setw(15) << "id"<<setw(15)<< "Tiempo"<<endl;
            for (int b = 0; b < bloqueado.size(); b++)
            {
                cout<<setw(15)<<bloqueado[b].id<<setw(15)<<bloqueado[b].contBloq<<endl;
            }
            
            global++;//Tiempo global***
            transcurrido++;
            
            if (kbhit()) 
            {
                key = getch();
                key = toupper(key);
                if (key == 'I') 
                {
                    proceso.porcentaje = i;
                    proceso.tiempo_trans = transcurrido;
                    proceso.estado = "Bloqueado";
                    bloqueado.push_back(proceso);
                    break;  // Salir para volver a ejecutar
                } else if (key == 'P') {
                    proceso.estado = "Pausa";
                    pausa();
                    proceso.estado = "Ejecución";  // Regresar a ejecución
                } else if (key == 'E') {
                    proceso.resultado = "Error";
                    proceso.estado = "Error";  // Cambiar estado a error
                    break;
                }
            }

            if (!bloqueado.empty()) 
            {
                
                for (int j = 0; j < bloqueado.size(); j++) 
                {
                    bloqueado[j].contBloq++;  // Incrementar el tiempo de bloqueo por 1 segundo
                    if (bloqueado[j].contBloq >= 7) 
                    {  
                        // Mover proceso bloqueado a memoria
                        bloqueado[j].estado = "Listo";
                        memoria.push(bloqueado[j]);
                        bloqueado.erase(bloqueado.begin() + j);
                        j--;  // Decrementar el índice ya que se ha eliminado un elemento
                    }
                }
            }
            Sleep(1000);  // Simular tiempo de ejecución
            system("cls");

        }

        // system("cls");
        if (key != 'I') 
        {
            proceso.tServicio = proceso.tiempo_max;
            proceso.estado = "Terminado";  // Estado final si no fue interrumpido
            proceso.tSalida = global; //Tiempo de salida***
            proceso.tRtrn = proceso.tSalida - proceso.tLlegada; //Tiempo de retorno***
            proceso.tEspera = proceso.tRtrn - proceso.tServicio;
            termi.push_back(proceso);
        }
    }  

    cout << "\nTerminados: "<<termi.size() << endl;

    cout << left << setw(15) << "Numero" << setw(18) <<"Llegada" <<setw(18) <<"Salida"<<setw(18)
    <<"Retorno"<<setw(18) <<"Respuesta"<<setw(18) <<"Espera"<<setw(18)
    <<"Servicio"<<setw(18) << "Resultado"<< "\tContador global: " << global << endl;
    for (int j = 0; j < termi.size(); j++)
    {
        cout << setw(15) << termi[j].id << setw(18) << termi[j].tLlegada<< setw(18) <<termi[j].tSalida<< setw(18)
        <<termi[j].tRtrn<< setw(18) <<termi[j].tResp<< setw(18) <<termi[j].tEspera<< setw(18)
        <<termi[j].tServicio<< setw(18) << termi[j].resultado << endl;
    }
    cout << endl;
    cout << "Ejecutado con exito" << endl;
    getch();
}

int main() 
{
    int num_procesos;
    queue<Lote> lotes;
    Lote loteAct;

    cout << "Numero de procesos: ";
    cin >> num_procesos;

    vector<Proceso> procesos = generar_procesos(num_procesos);
    mostrar(procesos);  // Pasar la cola de lotes a la función

    return 0;
}