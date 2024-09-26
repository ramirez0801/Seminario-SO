#include <iostream>
#include <iomanip>
#include <windows.h>
#include <conio.h>
#include <thread>
#include <chrono>

#include <unistd.h> // Para la función usleep

using namespace std;

int main() {
    int counter = 0;

    while (counter < 3) 
    {
        system("cls"); // Limpiar la pantalla
        std::cout << "Contador: " << counter << std::endl;
        counter++;
        string nombres[] = {"Juan", "Ana", "Pedro"};
        int edades[] = {25, 30, 22};
        float alturas[] = {1.75, 1.60, 1.80};

        cout << left << setw(10) << "Nombre" << setw(10) << "Edad" << setw(10) << "Altura" <<"\t\t\tTiempo: "<<"10"<< endl;
        for (int i = 0; i < counter; i++) {
            cout << setw(10) <<nombres[i] << setw(10) << edades[i] << setw(10) << fixed << setprecision(2) << alturas[i] <<endl;
        }

        std::this_thread::sleep_for(std::chrono::seconds(2)); // Esperar 1 segundo
        cout<<endl;
        for (int i = 0; i <= 100; ++i) 
        {
            cout << "\rProgreso: [" << string(i, '=') << string(100 - i, ' ') << "] " << i << "%";
            cout.flush();
            Sleep(20); // Esperar 50ms
        }
    
    }
    

    // int segundos;
    // while(segundos != -1)
    // {
    //     string nombres[] = {"Juan", "Ana", "Pedro"};
    //     int edades[] = {25, 30, 22};
    //     float alturas[] = {1.75, 1.60, 1.80};



    //     cout << left << setw(10) << "Nombre" << setw(10) << "Edad" << setw(10) << "Altura" <<"\t\t\tTiempo: "<<"10"<< endl;
    //     for (int i = 0; i < 3; ++i) {
    //         cout << setw(10) <<nombres[i] << setw(10) << edades[i] << setw(10) << fixed << setprecision(2) << alturas[i] <<endl;
    //     }

    //     cout<<"Segundos :";
    //     cin>>segundos;
    //     segundos = (segundos * 0.80) * 10; 
    //     cout<<"Segundos total "<<segundos<<endl;
    //     for (int i = 0; i <= 100; ++i) {
    //         cout << "\rProgreso: [" << string(i, '=') << string(100 - i, ' ') << "] " << i << "%";
    //         cout.flush();
    //         Sleep(segundos); // Esperar 50ms
    //     }
    //     cout << endl;
    // }

    // cout << "Hola Mundo!!!";
    //  Sleep(2000);
    // cout << "\rBuenos dias!!!";

    return 0;
}