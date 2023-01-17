#include <iostream>
#include <vector>
#include <chrono>
using namespace std;

#include "city.h"
#include "salesman_bruteforce.h"
#include "salesman_dynamic.h"
#include "salesman_branch.h"
#include "salesman_annealing.h"
#include "salesman_taboo.h"
#include "salesman_ant.h"
#include "salesman_genetic.h"

int main()
{   
    string version;
    cout << "Podaj \"random\" dla losowych danych, lub nazwe pliku: ";
    cin >> version;
    while (1) {
        int time = 0;

        random_device rd;
        try
        {
            if (version == "random")
            {
                int n;
                int r;
                cout << "Podaj liczbe wezlow: ";
                cin >> n;
                cout << "Podaj liczbe powtorzen: ";
                cin >> r;

                
                chrono::high_resolution_clock::time_point t1;
                chrono::high_resolution_clock::time_point t2;
                // testujemy dla:
                // 10
                // 14
                // 16
                //pierwszy parametr - liczba miast
                vector<city> test = generate_cities(n, 0, 100, 0, 100, rd);
                t1 = chrono::high_resolution_clock::now();
                for (int i = 0; i < r; i++) salesman_bruteforce(test);
                t2 = chrono::high_resolution_clock::now();
                cout << "Bruteforce time: " << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count() / r << "ms" << endl;
                cout << endl;
                t1 = chrono::high_resolution_clock::now();
                for (int i = 0; i < r; i++) salesman_branch(test);
                t2 = chrono::high_resolution_clock::now();
                cout << "B&B ver. A time: " << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count() / r << "ms" << endl;
                cout << endl;
                t1 = chrono::high_resolution_clock::now();
                for (int i = 0; i < r; i++) salesman_branch(test, true);
                t2 = chrono::high_resolution_clock::now();
                cout << "B&B ver. B time: " << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count() / r << "ms" << endl;
                cout << endl;
                t1 = chrono::high_resolution_clock::now();
                for (int i = 0; i < r; i++)  salesman_dynamic(test);
                t2 = chrono::high_resolution_clock::now();
                cout << "DP time: " << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count() / r << "ms" << endl;
            }
            else {
                ifstream p(version);
                vector<vector<int>> test = read_distances(p);
                p.close();
                vector<unsigned int> solution;
                chrono::high_resolution_clock::time_point t1;
                chrono::high_resolution_clock::time_point t2;
                cout << "Wybierz:" << endl << "1 - Dla przegladu zupelnego" << endl << "2 - Dla B&B" << endl << "3 - Dla programowania dynamicznego" << endl << "4 - Dla symulowanego wyzarzania" << endl << "5 - Dla taboo search" << endl << "6 - Dla zmiany pliku" << endl << "7 - Dla taboo i SA" << endl;
                int x;
                cin >> x;
                switch (x)
                {
                case(1): {
                    t1 = chrono::high_resolution_clock::now();
                    solution = salesman_bruteforce(test);
                    t2 = chrono::high_resolution_clock::now();
                    cout << "Bruteforce time: " << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count() << "ms" << endl;
                    cout << "Sciezka: " << solution << endl;
                    cout << "Dlugosc: " << path_length(test, solution) << endl;
                    cout << endl;
                    break;
                }
                case(2): {
                    t1 = chrono::high_resolution_clock::now();
                    solution = salesman_branch(test);
                    t2 = chrono::high_resolution_clock::now();
                    cout << "B&B ver. A time: " << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count() << "ms" << endl;
                    cout << "Sciezka: " << solution << endl;
                    cout << "Dlugosc: " << path_length(test, solution) << endl;
                    cout << endl;
                    break;
                }
                case(3): {
                    t1 = chrono::high_resolution_clock::now();
                    solution = salesman_dynamic(test);
                    t2 = chrono::high_resolution_clock::now();
                    cout << "DP time: " << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count() << "ms" << endl;
                    cout << "Sciezka: " << solution << endl;
                    cout << "Dlugosc: " << path_length(test, solution) << endl;
                    cout << endl;
                    break;
                }
                case(4): {
                    cout << "Podaj czas w sekundach: ";
                    cin >> time;
                    cout << endl;
                    solution = salesman_annealing(test, rd, time*1000);
                    cout << "Sciezka: " << solution << endl;
                    cout << "Dlugosc: " << path_length(test, solution) << endl;
                    break;

                }
                case(5): {
                    cout << "Podaj czas w sekundach: ";
                    cin >> time;
                    cout << endl;
                    solution = salesman_taboo(test, rd, time*1000);
                    cout << "Sciezka: " << solution << endl;
                    cout << "Dlugosc: " << path_length(test, solution) << endl;
                    break;
                }
                case(7): {
                    double ang = 0;
                    double taboo = 0;
                    cout << "Podaj czas w sekundach: ";
                    cin >> time;
                    cout << endl;
                    cout << "Podaj ilość powtorzen petli: ";
                    int j = 0;
                    cin >> j;
                    for (int i = 0; i < j; i++) {
                        solution = salesman_taboo(test, rd, time*1000);
                        cout << "Sciezka: " << solution << endl;
                        cout << "Dlugosc taboo: " << path_length(test, solution) << endl;
                        taboo += path_length(test, solution);
                        solution = salesman_annealing(test, rd, time*1000);
                        cout << "Sciezka: " << solution << endl;
                        cout << "Dlugosc: " << path_length(test, solution) << endl;
                        ang += path_length(test, solution);
                    }
                    taboo = taboo / j;
                    cout <<"Taboo srednio: " << taboo << endl;
                    ang = ang / j;
                    cout << "Ang srednio: " << ang << endl;
                    break;
                }
                case(6):{cout << "Podaj \"random\" dla losowych danych, lub nazwe pliku: ";
                    cin >> version; break; }
                }

            }
        }
        catch (exception& ex)
        {
            cout << "Blad:" << endl;
            cout << ex.what() << endl;
        }
        catch (...)
        {
            cout << "Nieznany blad" << endl;
        }

        system("pause");
    }
    
}
