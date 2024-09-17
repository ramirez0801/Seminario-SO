#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <conio.h>

class Proceso {

public:
    int id;
    int tiempo_max;
    char operacion;
    int operandos[2];
    std::string resultado;
    std::string estado;

    Proceso(int id, int tiempo_max, char operacion, int op1, int op2)
        : id(id), tiempo_max(tiempo_max), operacion(operacion) {
        operandos[0] = op1;
        operandos[1] = op2;
        estado = "En espera";
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

std::vector<Proceso> generar_procesos(int num_procesos) {
    std::vector<Proceso> procesos;
    char operaciones[] = {'+', '-', '*', '/', '%'};
    std::srand(std::time(0));
    for (int i = 0; i < num_procesos; ++i) {
        int id = i + 1;
        int tiempo_max = std::rand() % 16 + 5;
        char operacion = operaciones[std::rand() % 5];
        int op1 = std::rand() % 100 + 1;
        int op2 = std::rand() % 100 + 1;
        procesos.emplace_back(id, tiempo_max, operacion, op1, op2);
    }
    return procesos;
}

void ejecutar_procesos(std::vector<Proceso>& procesos) {
    char comando;
    for (auto& proceso : procesos) {
        proceso.estado = "En ejecución";
        std::cout << "Ejecutando proceso " << proceso.id << "...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(proceso.tiempo_max * 100));
        proceso.ejecutar();
        std::cout << "Proceso " << proceso.id << " terminado con resultado: " << proceso.resultado << "\n";
        
        if(kbhit())
        {
            std::cout << "Presiona 'I' para interrupción, 'E' para error, 'P' para pausa, 'C' para continuar: ";
            std::cin >> comando;
            comando = std::toupper(comando);
            if (comando == 'I') {
                interrupcion(procesos);
            } else if (comando == 'E') {
                error(procesos);
            } else if (comando == 'P') {
                pausa();
            } else if (comando == 'C') {
                continue;
            }
        }
    }
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

void error(std::vector<Proceso>& procesos) {
    if (!procesos.empty()) {
        Proceso proceso = procesos.front();
        procesos.erase(procesos.begin());
        proceso.estado = "Error";
        proceso.resultado = "Error";
        std::cout << "Proceso " << proceso.id << " terminado por error.\n";
    }
}

void pausa() {
    std::cout << "Ejecución pausada. Presiona 'C' para continuar.\n";
    while (true) {
        if (std::cin.get() == 'C') {
            break;
        }
    }
}

int main() {
    int num_procesos;
    std::cout << "Número de proceos inicial: ";
    std::cin >> num_procesos;

    std::vector<Proceso> procesos = generar_procesos(num_procesos);

    while (!procesos.empty()) {
        ejecutar_procesos(procesos);
        
        
        
    }

    return 0;
}
