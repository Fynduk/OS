#include <QCoreApplication>
#include <iostream>
#include <locale.h>
#include <stdio.h>
#include <math.h>
#include <QChar>
using namespace std;

//const int TAB_NAME = 10;
//const int TAB_TACT = 4;
struct process{
    char name;
    int tact;
    int time;
    int Pr0;
    int a;
};

struct ready{
    int n_proc; //номер процесса из списка процессов
    float Pr; //приоритет данного процесса
};

void sort(ready *a, int n);

int main(int argc, char *argv[])

{
    QCoreApplication a(argc, argv);
    setlocale(LC_ALL, "Russian");
    int rand_value;
    int number_of_processes;
    int sampling_interval;
    cout << "enter num of proccess - ";
    cin >> number_of_processes;
    cout << "interval percheta - ";
    cin >> sampling_interval;
    cout << "num for srand() - ";
    cin >> rand_value;
    srand(rand_value);
    process *process_array = new process[number_of_processes];
    ready *ready_for_implementation = new ready[number_of_processes + 1]; //готовые к выполнению процессы
    bool flag = true; //флаг работы процессора, если все процессы выполнены flag = false и закончить работу
    int tact = 0; //такт процессора
    int max_tact = 0; //максимальный так с которого како-то процесс готов к выполнению (для рандомного заполнения нужно)
    int n_ready = 0; //кол-во процессов готовых к выполнению
    for (int i = 0; i < number_of_processes + 1; i++) //заполнение -1
        ready_for_implementation[i].n_proc = -1;
    for (int i = 0; i < number_of_processes; i++) //задаются параметры для каждого процесса (рандомайзер)
    {
        process_array[i].name = 65 + i;
        process_array[i].time = 1 + rand() % 5;
        process_array[i].Pr0 = 1 + rand() % 5;
        process_array[i].a = 1 + rand() % 5;
        if (i == 0)
            process_array[i].tact = 0;
        else
        {
            process_array[i].tact = rand() % 5 + max_tact;
            if (max_tact < process_array[i].tact)
                max_tact = process_array[i].tact;
        }
    }
    cout << endl << " Name\t| Tact\t| Time\t| Pr0\t| a" << endl; //вывод таблицы всех процессов и их данных
    for (int i = 0; i < number_of_processes; i++)
        cout << " "
             << process_array[i].name << "\t| "
             << process_array[i].tact << "\t| "
             << process_array[i].time << "\t| "
             << process_array[i].Pr0 << "\t| "
             << process_array[i].a << endl;
    cout << endl << " | CPU | t \t| Ready" << endl; //вывод заголовка таблицы работы диспетчеризации
    for (tact = 0; flag; tact++) //работа дистепчеризации пока не кончатся процессы
    {
        for (int i = 0; i < number_of_processes; i++)
        {
            if (process_array[i].tact == tact) //добавление всех процессов, которые на данный так готовы к выполнению
            {
                process_array[i].tact = -1; //процесс попал в список готовых к выполнению
                ready_for_implementation[n_ready++].n_proc = i;
            }
        }

        if (tact != 0 && tact % sampling_interval == 0) //пересчет в нужный такт (0 убрали так как log(0) = нет решения)
        {
            for (int i = 0; i < n_ready; i++)
                ready_for_implementation[i].Pr = process_array[ready_for_implementation[i].n_proc].Pr0 + process_array[ready_for_implementation[i].n_proc].a * exp(tact);
            sort(ready_for_implementation, n_ready); //сортировка по убыванию
        }
        process_array[ready_for_implementation[0].n_proc].time--; //в 0 хранится номер процесса у которого сейчас больший приоритет, он и выполнется, уменьшается его время выполнения
        if (ready_for_implementation[0].n_proc == -1) //если вдруг нет готовых процессов
            cout << " | | " << tact << "\t|" << endl; //то вывод пустых строк
        else
        { //иначе выводим сначала имя процесса который работает
            cout << " | " << process_array[ready_for_implementation[0].n_proc].name << " | " << tact << "\t| ";
            for (int i = 1; i < n_ready; i++)//а дальше выводим все процессы, которые готовы к выполнению
                cout << process_array[ready_for_implementation[i].n_proc].name << " | ";
            cout << endl;
        }

        if (process_array[ready_for_implementation[0].n_proc].time == 0) //если выполнение процесса закончилось
        {
            for (int i = 0; i < n_ready; i++)//сдвиг массива, чтобы на 0 место встал номер процесса приоритет которого теперь самый высокий
                ready_for_implementation[i] = ready_for_implementation[i + 1];
            if (n_ready > 0) //чтобы не уходило меньше 0
                n_ready--;
        }

        for (int i = 0; i < number_of_processes + 1; i++) //проверка, остались ли еще готовые к выполнению процессы
        {
            if (ready_for_implementation[i].n_proc != -1)
            {
                flag = 1;
                break;
            }
            flag = 0;
        }
        if (!flag) //если к выполнению процессов нет, то проверим вдруг есть еще не готовые к выполнению процессы
        {
            for (int i = 0; i < number_of_processes; i++)
                if (process_array[i].tact >= 0)
                {
                    flag = 1;
                    break;
                }
        }
    }
    cout << endl;
    return a.exec();
}

void sort(ready *a, int n) //сортировка приоритета по убыванию
{
    int i, k;
    ready z;
    for (int k = n; k > 0; k--)
        for (int i = 0; i < k - 1; i++)
            if (a[i].Pr < a[i + 1].Pr)
            {
                z = a[i];
                a[i] = a[i + 1];
                a[i + 1] = z;
            }
}
