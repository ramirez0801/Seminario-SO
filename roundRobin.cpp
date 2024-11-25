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

int idss = 1;

class Proceso { //Clase del proceso
public:
    int id;
    int tiempo_max, tiempo_trans, porcentaje, tLlegada, tSalida, tRtrn, tResp, tEspera, tServicio, contBloq, quantum;
    char operacion;
    int operandos[2];
    string resultado;
    string estado;

    Proceso() : id(0), tiempo_max(0), operacion('+') 
    { 
        operandos[0] = 0; 
        operandos[1] = 0; 
        estado = "Nuevo"; 
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
        quantum = 0;
    }

    void ejecutar() {
        estado = "Ejecucion"; // Cambia el estado a ejecución al iniciar
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


vector<Proceso> generar_procesos(int num_procesos) {
    vector<Proceso> procesos;
    char operaciones[] = {'+', '-', '*', '/', '%'};
    srand(static_cast<unsigned int>(time(0)));
    for (int i = 0; i < num_procesos; ++i) {
        int id = idss;
        int tiempo_max = rand() % 16 + 5;
        char operacion = operaciones[rand() % 5];
        int op1 = rand() % 100 + 1;
        int op2 = rand() % 100 + 1;
        idss++;
        procesos.emplace_back(id, tiempo_max, operacion, op1, op2);
    }
    return procesos;
}

void pausa() {
    char key;
    while (true) {
        if (kbhit()) 
            { 
                key = getch(); 
                key = toupper(key); 
                if (key == 'C') 
                    break; 
            } 
    }
}

void mostrar(vector<Proceso>& procesos) 
{
    char key;
    int global = 0, quantum = 0;
    vector<Proceso> termi;        // Lista de procesos terminados
    vector<Proceso> bloqueado;   // Lista de procesos bloqueados
    queue<Proceso> memoria;      // Cola de procesos en memoria (listos para ejecucion)
    vector<Proceso> procEjecucion; // Proceso en ejecucion (maximo 1)

    cout<<"Tiempo para el quantum\n";
    cin>>quantum;

    while (!procesos.empty() || !memoria.empty() || !bloqueado.empty() || !procEjecucion.empty()) 
    {
        // Cargar procesos a memoria si hay espacio disponible
        while (!procesos.empty() && (memoria.size() + bloqueado.size() + procEjecucion.size()) < 5) 
        {
            procesos.front().estado = "Listo";
            procesos.front().tLlegada = global;  // Tiempo de llegada
            memoria.push(procesos.front());
            procesos.erase(procesos.begin());
        }

        // Mover procesos bloqueados de vuelta a memoria si cumplen el tiempo de bloqueo
        for (int i = 0; i < bloqueado.size(); i++) {
            bloqueado[i].contBloq++;
            if (bloqueado[i].contBloq >= 7 && (memoria.size() + procEjecucion.size()) < 5) {
                bloqueado[i].estado = "Listo";
                bloqueado[i].contBloq = 0;
                memoria.push(bloqueado[i]);
                bloqueado.erase(bloqueado.begin() + i);
                i--;
            }
        }

        // Si no hay procesos en ejecucion, toma el siguiente de memoria
        if (procEjecucion.empty() && !memoria.empty()) {
            procEjecucion.push_back(memoria.front());
            memoria.pop();
            procEjecucion.front().estado = "Ejecucion";
            procEjecucion.front().ejecutar();
            procEjecucion.front().tResp = global - procEjecucion.front().tLlegada;  // Tiempo de respuesta
        }

        // Todos los procesos estan bloqueados
        if (procEjecucion.empty() && memoria.empty() && !bloqueado.empty()) {
            cout << "\nTodos los procesos estan bloqueados. Esperando desbloqueo..." << endl;

            // Incrementar tiempo de bloqueo de todos los procesos
            for (int i = 0; i < bloqueado.size(); i++) {
                bloqueado[i].contBloq++;
                if (bloqueado[i].contBloq >= 7 && (memoria.size() + procEjecucion.size()) < 5) {
                    bloqueado[i].estado = "Listo";
                    bloqueado[i].contBloq = 0;
                    memoria.push(bloqueado[i]);
                    bloqueado.erase(bloqueado.begin() + i);
                    i--;
                }
            }

           
            global++;
            Sleep(1000);
            continue;  // Volver a la siguiente iteracion del ciclo principal
        }

        // Simulacion de ejecucion del proceso actual
        if (!procEjecucion.empty()) {
            Proceso& procesoActual = procEjecucion.front();
            procesoActual.tiempo_trans++;
            procesoActual.quantum++;
            // Mostrar estado actual del sistema
            system("cls");
            cout << "Tiempo global: " << global << endl;

            // **Mostrar contador de procesos en "Nuevos"**
            cout << "\nProcesos en 'Nuevos': " << procesos.size() << endl;

            cout << "Quantum: "<<quantum<<endl;

            // **Imprimir proceso en ejecucion**
            cout << "\nProceso en ejecucion:" << endl;
            cout << left << setw(10) << "ID" << setw(15) << "Estado" << setw(15) << "Operacion"
                 << setw(15) << "Tiempo Max" << setw(15) << "Tiempo Trans" << endl;
            cout << setw(10) << procesoActual.id
                 << setw(15) << procesoActual.estado
                 << setw(15) << procesoActual.operacion
                 << setw(15) << procesoActual.tiempo_max
                 << setw(15) << procesoActual.tiempo_trans << endl;

            // **Imprimir procesos en memoria**
            cout << "\nProcesos en memoria:" << endl;
            cout << left << setw(10) << "ID" << setw(15) << "Estado" << setw(15) << "Operacion" << setw(15) << "Transcurrido"<< endl;

            // Procesos en la cola de memoria
            queue<Proceso> memoriaAux = memoria;  // Copia auxiliar para iterar
            while (!memoriaAux.empty()) {
                Proceso temp = memoriaAux.front();
                memoriaAux.pop();
                cout << setw(10) << temp.id << setw(15) << temp.estado << setw(15) << temp.operacion <<setw(15) << temp.tiempo_trans << endl;
            }

            // **Imprimir procesos bloqueados**
            cout << "\nProcesos bloqueados:" << endl;
            cout << left << setw(10) << "ID" << setw(15) << "Estado" << setw(15) << "Tiempo Bloq" << endl;
            for (const auto& proc : bloqueado) {
                cout << setw(10) << proc.id << setw(15) << proc.estado << setw(15) << proc.contBloq << endl;
            }

            // **Imprimir procesos terminados**
            cout << "\nProcesos terminados:" << endl;
            cout << left << setw(10) << "ID" << setw(15) << "Estado" << setw(15) << "Resultado" << endl;
            for (const auto& proc : termi) {
                cout << setw(10) << proc.id << setw(15) << proc.estado << setw(15) << proc.resultado << endl;
            }

            // Manejar entrada de usuario
            if (kbhit()) {
                key = toupper(getch());
                if (key == 'I') {
                    // Bloquear el proceso actual
                    procesoActual.estado = "Bloqueado";
                    bloqueado.push_back(procesoActual);
                    procEjecucion.pop_back();
                } else if (key == 'P') {
                    // Pausar el proceso actual
                    procesoActual.estado = "Pausa";
                    cout << "\nProceso en pausa. Presiona cualquier tecla para continuar...";
                    pausa();
                    procesoActual.estado = "Ejecucion";
                } else if (key == 'E') {
                    // Finalizar con error
                    procesoActual.estado = "Error";
                    procesoActual.resultado = "Error";
                    termi.push_back(procesoActual);
                    procEjecucion.pop_back();
                } 
                else if (key == 'N') 
                {
                    // Crear un nuevo proceso
                    vector<Proceso> nuevoProceso = generar_procesos(1); // Genera un proceso
                    cout << "Proceso generado: " << nuevoProceso.front().id << endl;

                    // Verificar si hay espacio en memoria
                    if ((memoria.size() + bloqueado.size() + procEjecucion.size()) < 5) {
                        // Espacio disponible, añadir a memoria
                        nuevoProceso.front().estado = "Listo";
                        nuevoProceso.front().tLlegada = global; 
                        memoria.push(nuevoProceso.front());
                        cout << "Proceso " << nuevoProceso.front().id << " agregado a memoria." << endl;
                    } else {
                        // No hay espacio en memoria, añadir a procesos nuevos
                        nuevoProceso.front().estado = "Nuevo";
                        procesos.push_back(nuevoProceso.front());
                        cout << "Memoria llena, proceso " << nuevoProceso.front().id << " agragado a procesos nuevos." << endl;
                    }
                }

                else if(key == 'T')
                {
                    cout<<endl;
                    cout<<left<<setw(10)<<"ID"<<setw(10)<<"Estado"<<setw(10)<<"Operacion"<<setw(10)<<"Dato 1"<<setw(10)<<"Dato 2"<<setw(10)<<"Result"
                    <<setw(10)<<"Llegada"<<setw(10)<<"Fin"<<setw(10)<<"Retorno"<<setw(10)<<"Espera"<<setw(10)<<"Servicio"<<setw(10)<<"Resta"<<setw(10)<<"Respuesta"
                    <<setw(10)<<"Bloqueado\n";
                    
                    if(!procesos.empty())
                    {
                        for(int i = 0; i < procesos.size(); i++)
                        {
                            cout<<setw(10)<<procesos[i].id<<setw(10)<<procesos[i].estado<<setw(10)<<procesos[i].operacion<<setw(10)<<procesos[i].operandos[0]<<setw(10)<<procesos[i].operandos[1]
                            <<setw(10)<<"N/A"<<setw(10)<<"N/A"<<setw(10)<<"N/A"<<setw(10)<<"N/A"<<setw(10)<<"N/A"<<setw(10)<<"N/A"<<setw(10)<<"N/A"<<setw(10)<<"N/A"<<setw(10)<<"N/A"<<endl;
                        }
                    }

                    cout<<setw(10)<<procEjecucion.front().id<<setw(10)<<procEjecucion.front().estado<<setw(10)<<procEjecucion.front().operacion<<setw(10)<<procEjecucion.front().operandos[0]<<setw(10)<<procEjecucion.front().operandos[1]<<
                    setw(10)<<"N/A"<<setw(10)<<procEjecucion.front().tLlegada<<setw(10)<<procEjecucion.front().tSalida<<setw(10)<<procEjecucion.front().tRtrn<<setw(10)<<procEjecucion.front().tEspera<<setw(10)<<procEjecucion.front().tServicio
                    <<setw(10)<<procEjecucion.front().tiempo_max - procEjecucion.front().tiempo_trans<<setw(10)<<procEjecucion.front().tResp<<setw(10)<<"N/A"<<endl;

                    if(!termi.empty())
                    {
                        for(int j = 0; j < termi.size(); j++)
                        {
                            cout<<setw(10)<<termi[j].id<<setw(10)<<termi[j].estado<<setw(10)<<termi[j].operacion<<setw(10)<<termi[j].operandos[0]<<setw(10)<<termi[j].operandos[1]<<setw(10)<<termi[j].resultado
                            <<setw(10)<<termi[j].tLlegada<<setw(15)<<termi[j].tSalida<<setw(10)<<termi[j].tRtrn<<setw(10)<<termi[j].tEspera<<setw(10)<<termi[j].tServicio
                            <<setw(10)<<termi[j].tiempo_max - termi[j].tiempo_trans<<setw(10)<<termi[j].tResp<<setw(10)<<"N/A"<<endl;     
                        }
                    }

                    if(!bloqueado.empty())
                    {
                        for(int j = 0; j < bloqueado.size(); j++)
                        {
                            cout<<setw(10)<<bloqueado[j].id<<setw(10)<<bloqueado[j].estado<<setw(10)<<bloqueado[j].operacion<<setw(10)<<bloqueado[j].operandos[0]<<setw(10)<<bloqueado[j].operandos[1]<<setw(10)<<"N/A"
                            <<setw(10)<<bloqueado[j].tLlegada<<setw(10)<<bloqueado[j].tSalida<<setw(10)<<bloqueado[j].tRtrn<<setw(10)<<bloqueado[j].tEspera<<setw(10)<<bloqueado[j].tServicio
                            <<setw(10)<<bloqueado[j].tiempo_max - bloqueado[j].tiempo_trans<<setw(10)<<bloqueado[j].tResp<<setw(10)<<7 - bloqueado[j].contBloq<<endl;
                            
                        }
                    }

                    if(!memoria.empty())
                    {
                        queue<Proceso> memoriaAux = memoria; 
                        while (!memoriaAux.empty()) 
                        {
                            Proceso temp = memoriaAux.front();
                            memoriaAux.pop();
                            cout << setw(10) << temp.id << setw(10) << temp.estado << setw(10) << temp.operacion << setw(10) <<temp.operandos[0]<< setw(10) <<temp.operandos[1] << setw(10) << "N/A"
                            << setw(10) << temp.tLlegada << setw(10) << temp.tSalida << setw(10) << temp.tRtrn << setw(10) <<temp.tEspera << setw(10) << temp.tServicio 
                            << setw(10) << temp.tiempo_max - temp.tiempo_trans << setw(10) << temp.tResp<< setw(10) << "N/A" <<  endl;
                        }
                    }
                    pausa();
                }
                
            }

            // Si el proceso actual termina su ejecucion
            if (procesoActual.tiempo_trans >= procesoActual.tiempo_max) 
            {
                procesoActual.estado = "Terminado";
                procesoActual.tServicio = procesoActual.tiempo_max;
                procesoActual.tSalida = global;
                procesoActual.tRtrn = procesoActual.tSalida - procesoActual.tLlegada;
                procesoActual.tEspera = procesoActual.tRtrn - procesoActual.tServicio;
                termi.push_back(procesoActual);
                procEjecucion.pop_back();
                cout<<"Guardado"<<endl;
            }

            if(procesoActual.quantum >= quantum)
            {
                procesoActual.quantum = 0;
                procesoActual.estado = "Listo";
                memoria.push(procesoActual);
                procEjecucion.pop_back();
            }
        }

        // Incrementar el tiempo global
        Sleep(1000);
        global++;
    } 

    // Mostrar resultados finales
    cout << "\nProcesos terminados:" << endl;
    cout << left << setw(15) << "ID" << setw(15) << "Llegada" << setw(15) << "Salida"
         << setw(15) << "Retorno" << setw(15) << "Respuesta" << setw(15) << "Espera"
         << setw(15) << "Servicio" << setw(15) << "Resultado" << endl;

    for (const auto& proceso : termi) {
        cout << setw(15) << proceso.id << setw(15) << proceso.tLlegada << setw(15) << proceso.tSalida
             << setw(15) << proceso.tRtrn << setw(15) << proceso.tResp << setw(15) << proceso.tEspera
             << setw(15) << proceso.tServicio << setw(15) << proceso.resultado << endl;
    }
    cout << "\nEjecucion completada con exito." << endl;
    getch();
}

int main() 
{
    int num_procesos;

    cout << "Numero de procesos: ";
    cin >> num_procesos;


    vector<Proceso> procesos = generar_procesos(num_procesos);
    mostrar(procesos);  // Pasar la cola de lotes a la función

    return 0;
}