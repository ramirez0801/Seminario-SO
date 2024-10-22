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
    int tiempo_max, tiempo_trans, porcentaje, bloqueado;
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
    std::cout << "Ejecución pausada. Presiona 'C' para continuar.\n";
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

    while (!procesos.empty()) {
        system("cls");
        //Lote lo = lotes.front();
        //lotes.pop();

        if(procesos.empty())
            continue;
        else if(memoria.size() != 5)
            for(r = r; memoria.size() < 5; r++) //Mantem: Revisar r
            {
                procesos[r].estado = "Listo";
                memoria.push(procesos[r]);
            }

        vector<Proceso> procesos_actuales = procesos;  // Copia de los procesos
        Proceso& proceso = memoria.front();  // Referencia al proceso


        // Ejecutar todos los procesos en el lote actual
        //for (size_t m = 0; m < procesos_actuales.size(); m++) {
            int tam = procesos_actuales.size();
            aux = 0;
            transcurrido = 0;
            
            // Mostrar información antes de ejecutar
            cout << endl;
            cout << left << setw(15) << "Numero" 
                 << setw(15) << "Tiempo"
                 << setw(15) << "Estado"
                 << endl;
            
            queue<Proceso> mem_copy = memoria;
            for (int i = 0; i < memoria.size(); i++) {
                cout << left << setw(15) << mem_copy.front().id
                     << setw(15) << procesos_actuales[i].tiempo_max
                     << setw(15) << procesos_actuales[i].estado << endl;
            }
            cout << "\nTerminados: " << endl;
            cout << left << setw(15) << "Numero" << setw(15) << "Operacion"
                 << setw(15) << "Resultado"
                 << "\t\t\tContador global: " << global / 100 << endl;
            for (int j = 0; j < termi.size(); j++) {
                cout << setw(15) << termi[j].id << setw(15) << termi[j].operacion
                     << setw(15) << termi[j].resultado << endl;
            }
            cout << endl;

            cout << "Ejecutando Proceso: " << proceso.id << endl;
            proceso.ejecutar();  // Ejecutar el proceso

            if (proceso.tiempo_trans != 0)
                aux = proceso.porcentaje;

            seg = proceso.tiempo_max * 10;  // Ajustar el tiempo para Sleep
            // Simulando tiempo de ejecución
            for (int i = aux; i <= 100; ++i) {
                key = '\0';
                cout << "\rProgreso: [" << string(i, '=') << string(100 - i, ' ')<< "] " << i << "%";
                cout.flush();
                Sleep(seg);  // Simular tiempo de ejecución
                global = global + (seg / 10);
                transcurrido = transcurrido + (seg / 10);

                if (kbhit()) {
                    //cout << "\nPresiona 'I' para interrupción, 'P' para pausa, 'E' para terminar con error: ";
                    cin >> key;
                    key = toupper(key);
                    if (key == 'I') {
                        cout << "Proceso " << proceso.id << " interrumpido, pasa a estado bloqueado.\n";
                        proceso.porcentaje = i;
                        proceso.tiempo_trans = transcurrido / 100;
                        proceso.estado = "Interrumpido";  // Cambiar estado a interrumpido
                       // procesos_actuales.erase(procesos_actuales.begin() + m);
                        for(int i = 0; i <= 100; i++)
                        {
                            cout << "\rProgreso: [" << string(i, '=') << string(100 - i, ' ')<< "] " << i << "%";
                            cout.flush();
                            Sleep(70);
                        }
                        proceso.bloqueado += 7;
                        proceso.estado = "Bloqueado";
                        memoria.push(proceso);
                        bloqueado.push_back(proceso);
                        //m--;  // Ajustar índice después de eliminar
                        break;  // Salir para volver a ejecutar el lote
                    } else if (key == 'P') {
                        proceso.estado = "Pausa";  // Cambiar estado a bloqueado
                        pausa();
                        proceso.estado = "Ejecución";  // Regresar a ejecución
                    } else if (key == 'E') {
                        proceso.resultado = "Error";
                        proceso.estado = "Error";  // Cambiar estado a error
                        cout << "Proceso " << proceso.id << " terminado con error.\n";
                        break;
                    }
                }
            //}

            if (key != 'I') {
                proceso.estado = "Terminado";  // Estado final si no fue interrumpido
                termi.push_back(proceso);
                procesos_actuales.erase(procesos_actuales.begin());
                //m--;
            }
            system("cls");
        }

        cout << "\nLote completado con los procesos restantes.\n";
        cout << "Terminados: " << endl;
        cout << left << setw(15) << "Numero" << setw(15) << "Operacion"
             << setw(15) << "Resultado"
             << "\t\t\tContador global: " << global / 100 << endl;
        for (int j = 0; j < termi.size(); j++) {
            cout << setw(15) << termi[j].id << setw(15) << termi[j].operacion
                 << setw(15) << termi[j].resultado << endl;
        }
        cout << endl;
    }
    cout << "Ejecutado con éxito" << endl;
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

    mostrar(procesos);  // Pasar la cola de lotes a la función

    return 0;
}