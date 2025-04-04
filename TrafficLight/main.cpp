#include <iostream>
#include <chrono>
#include <thread>
#include <string>

using namespace std;
using namespace chrono;

const string GREEN = "ЗЕЛЁНЫЙ";
const string YELLOW = "ЖЁЛТЫЙ";
const string RED = "КРАСНЫЙ";

// Глобальная переменная для подсчёта прошедших секунд c начала работы программы
int secondsSinceStart = 1;

// Функция печати состояния светофоров и прошедших секунд c начала работы программы
void printConsol(const string light1, const string light2) {

    cout << "    Прошло секунд с запуска: " << secondsSinceStart << endl;
    cout << "  --------------------------------" << endl;
    cout << "  |" << "         ДЛЯ ОВЕЦ:            |" << endl;
    cout << "  --------------------------------" << endl;
    cout << "  |" << "   Светофор   |     " << light1 << "    |" << endl;
    cout << "  --------------------------------" << endl;
    cout << "  |" << "         ДДЛЯ КОРОВ:           |" << endl;
    cout << "  --------------------------------" << endl;
    cout << "  |" << "   Светофор   |     " << light2 << "    |" << endl;
    cout << "  --------------------------------" << endl << endl << endl;
}

// Функция, выполняющая фазу с заданной длительностью
void runPhase(const string light1, const string light2, int durationSeconds) {
    for (int i = 0; i < durationSeconds; ++i) {
        printConsol(light1, light2);
        this_thread::sleep_for(seconds(1));
        ++secondsSinceStart;
    }
}

int main() {
    setlocale(LC_ALL, "RUS");
    while (true) {
        runPhase(GREEN, RED, 15);      // Фаза, когда светофор для овец зеленый, для коров - красный
        runPhase(YELLOW, YELLOW, 5);  // Фаза, когда оба светофора жёлтые
        runPhase(RED, GREEN, 15);     // Фаза, когда светофор для овец красный, для коров - зеленый
        runPhase(YELLOW, YELLOW, 5);  // Фаза, когда оба светофора жёлтые
    }

    return 0;
}
