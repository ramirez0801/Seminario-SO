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
    int tiempo_max, tiempo_trans, porcentaje, bloqueado, tLlegada, tSalida, tRtrn, tResp, tEspera, tServicio;
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
        bloqueado = 0;
        tLlegada = 0;
        tSalida = 0;
        tRtrn = 0;
        tResp = 0;
        tEspera = 0;
        tServicio = 0;
    }

    void ejecutar() {
        estado = "Ejecuci�n"; // Cambia el estado a ejecuci�n al iniciar
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
                        throw std::runtime_error("Divisi�n por cero");
                    break;
                case '%':
                    resultado = std::to_string(operandos[0] % operandos[1]);
                    break;
                default:
                    throw std::runtime_error("Operaci�n desconocida");
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
    while (!procesos.empty() || !memoria.empty())
    {
        system("cls");
        //Lote lo = lotes.front();
        //lotes.pop();

        if(!procesos.empty() && memoria.size() != 5)
        {
            r = 0;
            while(r < procesos.size()) //Mantem: Revisar r
            {
                cout<<"Entra\n";
                procesos[r].estado = "Listo";
                procesos[r].tLlegada = global/100; //Tiempo llegada***
                memoria.push(procesos[r]);
                cout<<"Tamano de memoria: "<<memoria.size()<<endl;
                procesos.erase(procesos.begin() + r);
                cout<<"Tamano de procesos: "<<procesos.size()<<endl;
                if(memoria.size() == 5)
                {
                    cout<<"Memoria llena\n";
                    break;
                }
            }
        }

        //vector<Proceso> procesos_actuales = procesos;  // Copia de los procesos
        // Ejecutar todos los procesos
        //for (size_t m = 0; m < procesos_actuales.size(); m++) {
            //int tam = procesos_actuales.size();
            aux = 0;
            transcurrido = 0;

            // Mostrar informaci�n antes de ejecutar
            cout << endl;
            cout << left << setw(15) << "Numero" << setw(15) << "Tiempo"<< setw(15) << "Estado"<< endl;
            queue<Proceso> mem_copy = memoria;
            while(!mem_copy.empty())
            {
                cout << left << setw(15) << mem_copy.front().id<< setw(15) << mem_copy.front().tiempo_max<< setw(15) << mem_copy.front().estado << endl;
                mem_copy.pop();
            }

            cout << "\nTerminados: " << endl;
            cout << left << setw(15) << "Numero" << setw(15) << "Operacion"<< setw(15) << "Resultado" << "\t\t\tContador global: " << global / 100 << endl;
            for (int j = 0; j < termi.size(); j++)
            {
                cout << setw(15) << termi[j].id << setw(15) << termi[j].operacion<< setw(15) << termi[j].resultado << endl;
            }
            cout << endl;

            Proceso proceso = memoria.front();  // Referencia al proceso
            memoria.pop();

            cout << "Ejecutando Proceso: " << proceso.id << endl;
            proceso.ejecutar();  // Ejecutar el proceso

            if (proceso.tiempo_trans != 0)
                aux = proceso.porcentaje;

            cout<<"Tiempo: "<<proceso.tiempo_max<<endl;
            seg = proceso.tiempo_max * 10;  // Ajustar el tiempo para Sleep
            proceso.tResp = global / 100 - proceso.tLlegada; //Tiempo respuesta***
            cout<<"Resp: "<<proceso.tResp<<endl;
            // Simulando tiempo de ejecuci�n
            for (int i = aux; i <= 100; ++i)
            {
                key = '\0';
                cout << "\rProgreso: [" << string(i, '=') << string(100 - i, ' ')<< "] " << i << "%";
                cout.flush();
                Sleep(seg);  // Simular tiempo de ejecuci�n
                global = global + (seg / 10);//Tiempo global***
                transcurrido = transcurrido + (seg / 10);

                if (kbhit())
                {
                    //cout << "\nPresiona 'I' para interrupci�n, 'P' para pausa, 'E' para terminar con error: ";
                    key = getch();
                    key = toupper(key);
                    if (key == 'I')
                    {
                        cout << "\nProceso " << proceso.id << " interrumpido, pasa a estado bloqueado.\n";
                        proceso.porcentaje = i;
                        proceso.tiempo_trans = transcurrido / 100;
                        proceso.estado = "Interrumpido";  // Cambiar estado a interrumpido
                       // procesos_actuales.erase(procesos_actuales.begin() + m);
                        for(int i = 0; i <= 100; i++)
                        {
                            cout << "\rProgreso: [" << string(i, '=') << string(100 - i, ' ')<< "] " << i << "%";
                            cout.flush();
                            Sleep(70);
                            global = global + (seg / 10);
                        }
                        proceso.bloqueado += 7;
                        proceso.estado = "Bloqueado";
                        memoria.push(proceso);
                        bloqueado.push_back(proceso);
                        //m--;  // Ajustar �ndice despu�s de eliminar
                        break;  // Salir para volver a ejecutar
                    } else if (key == 'P') {
                        proceso.estado = "Pausa";  // Cambiar estado a bloqueado
                        pausa();
                        proceso.estado = "Ejecuci�n";  // Regresar a ejecuci�n
                    } else if (key == 'E') {
                        proceso.resultado = "Error";
                        proceso.estado = "Error";  // Cambiar estado a error
                        cout << "Proceso " << proceso.id << " terminado con error.\n";
                        break;
                    }
                }
            }

           system("cls");
        if (key != 'I')
        {
            proceso.tServicio = proceso.tiempo_max;
            proceso.estado = "Terminado";  // Estado final si no fue interrumpido
            proceso.tSalida = global/100; //Tiempo de salida***
            proceso.tRtrn = proceso.tSalida - proceso.tLlegada; //Tiempo de retorno***
            proceso.tEspera = proceso.tRtrn - proceso.tServicio;
            cout<<"\nretrono: "<<proceso.tRtrn<<endl;
            termi.push_back(proceso);
            cout<<"\nGuardado\n";
            //procesos_actuales.erase(procesos_actuales.begin());
            //m--;
        }
    }

    cout << "\nTerminados: "<<termi.size() << endl;
    cout << left << setw(15) << "Numero" << setw(18) <<"Llegada" <<setw(18) <<"Salida"<<setw(18)
    <<"Retorno"<<setw(18) <<"Respuesta"<<setw(18) <<"Espera"<<setw(18)
    <<"Servicio"<<setw(18) << "Resultado"<< "\t\tContador global: " << global / 100 << endl;
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

    // for (int i = 0; i < num_procesos; i++) {
    //     if (loteAct.pro.size() == 5) {
    //         lotes.push(loteAct);
    //         loteAct.pro.clear();
    //     }
    //     loteAct.pro.push_back(procesos[i]);
    // }
    // if (!loteAct.pro.empty()) {
    //     lotes.push(loteAct);
    // }

    mostrar(procesos);  // Pasar la cola de lotes a la funci�n

    return 0;
}
