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

// Предыдущие цвета светофоров
string prevLightSheep = RED;
string prevLightCow = RED;

// Получить статус заслонки
string getGateStatus(const string currentLight, const string prevLight) {
    if (currentLight == GREEN) return "ОТКРЫТА";
    if (currentLight == YELLOW && prevLight == GREEN) return "ОТКРЫТА";
    return "ЗАКРЫТА";
}

// Функция печати состояния светофоров и прошедших секунд c начала работы программы
void printConsol(const string lightSheep, const string lightCow) {

    string gateSheep = getGateStatus(lightSheep, prevLightSheep);
    string gateCow = getGateStatus(lightCow, prevLightCow);

    system("cls");

    cout << "    Прошло секунд с запуска: " << secondsSinceStart << endl;
    cout << "  --------------------------------" << endl;
    cout << "  |" << "          ДЛЯ ОВЕЦ:            |" << endl;
    cout << "  --------------------------------" << endl;
    cout << "  |" << "   Светофор   |     " << lightSheep << "    |" << endl;
    cout << "  --------------------------------" << endl;
    cout << "  |" << "   Заслонка   |     " << gateSheep << "    |" << endl;
    cout << "  --------------------------------" << endl;
    cout << "  |" << "          ДЛЯ КОРОВ:           |" << endl;
    cout << "  --------------------------------" << endl;
    cout << "  |" << "   Светофор   |     " << lightCow << "    |" << endl;
    cout << "  --------------------------------" << endl;
    cout << "  |" << "   Заслонка   |     " << gateCow << "    |" << endl;
    cout << "  --------------------------------" << endl << endl << endl;
}

// Функция, выполняющая фазу с заданной длительностью
void runPhase(const string lightSheep, const string lightCow, int durationSeconds) {
    for (int i = 0; i < durationSeconds; ++i) {
        printConsol(lightSheep, lightCow);
        this_thread::sleep_for(seconds(1));
        ++secondsSinceStart;
    }

    // Сохраняем текущие цвета как предыдущие
    prevLightSheep = lightSheep;
    prevLightCow = lightCow;
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
