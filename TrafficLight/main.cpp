#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <mutex>
#include <atomic>

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
string prevLightGoat = RED;


bool requestSheep = false;
bool requestCow = false;
bool requestGoat = false;

mutex requestMutex;
atomic<bool> running(true);

// Глобальная переменная для последнего нажавшего на кнопку
string lastRequester = "";

// Получить статус заслонки
string getGateStatus(const string currentLight, const string prevLight) {
    if (currentLight == GREEN) return "ОТКРЫТА";
    if (currentLight == YELLOW && prevLight == GREEN) return "ОТКРЫТА";
    return "ЗАКРЫТА";
}

// Функция печати состояния светофоров и прошедших секунд c начала работы программы
void printConsol(const string lightSheep, const string lightCow, const string lightGoat) {

    string gateSheep = getGateStatus(lightSheep, prevLightSheep);
    string gateCow = getGateStatus(lightCow, prevLightCow);
    string gateGoat = getGateStatus(lightGoat, prevLightGoat);

    system("cls");

    cout << "    Прошло секунд с запуска: " << secondsSinceStart << endl << endl;

    cout << "============================================================" << endl;
    cout <<"  ПОДСКАЗКА ДЛЯ ОПЕРАТОРА:" << endl;
    cout <<"  Нажмите [1] - открыть вольер с ОВЦАМИ" << endl;
    cout <<"  Нажмите [2] - открыть вольер с КОРОВАМИ" << endl;
    cout <<"  Нажмите [3] - открыть вольер с КОЗАМИ" << endl;
    cout << "============================================================" << endl << endl;


    cout << "  --------------------------------" << endl;
    cout << "  |" << "          ДЛЯ ОВЕЦ:            |" << endl;
    cout << "  --------------------------------" << endl;
    cout << "  |" << "   Светофор   |     " << lightSheep << "    |" << endl;
    cout << "  --------------------------------" << endl;
    cout << "  |" << "   Заслонка   |     " << gateSheep << "    |" << endl;
    cout << "  --------------------------------" << endl << endl;
    cout << "  --------------------------------" << endl;
    cout << "  |" << "          ДЛЯ КОРОВ:           |" << endl;
    cout << "  --------------------------------" << endl;
    cout << "  |" << "   Светофор   |     " << lightCow << "    |" << endl;
    cout << "  --------------------------------" << endl;
    cout << "  |" << "   Заслонка   |     " << gateCow << "    |" << endl;
    cout << "  --------------------------------" << endl << endl;
    cout << "  --------------------------------" << endl;
    cout << "  |" << "          ДЛЯ КОЗ:             |" << endl;
    cout << "  --------------------------------" << endl;
    cout << "  |   Светофор   |   " << lightGoat << "      |" << endl;
    cout << "  --------------------------------" << endl;
    cout << "  |   Заслонка   |   " << gateGoat << "      |" << endl;
    cout << "  --------------------------------" << endl << endl;

    cout << "  Активный вольер: "
        << (lightSheep == GREEN || lightSheep == YELLOW ? "ОВЦЫ" :
            lightCow == GREEN || lightCow == YELLOW ? "КОРОВЫ" :
            lightGoat == GREEN || lightGoat == YELLOW ? "КОЗЫ" : "НЕТ") << endl;

}

// Функция, выполняющая фазу с заданной длительностью
void runPhase(const string lightSheep, const string lightCow, const string lightGoat, int durationSeconds) {
    for (int i = 0; i < durationSeconds; ++i) {
        printConsol(lightSheep, lightCow, lightGoat);
        this_thread::sleep_for(seconds(1));
        ++secondsSinceStart;
    }

    // Сохраняем текущие цвета как предыдущие
    prevLightSheep = lightSheep;
    prevLightCow = lightCow;
    prevLightGoat = lightGoat;
}


void inputThreadFunc() {
    while (running) {
        char key;
        cin >> key;

        lock_guard<mutex> lock(requestMutex);
        if (key == '1') {
            requestSheep = true;
            lastRequester = "sheep";
            cout << "[Запрос] Овцы хотят выйти!" << endl;
        }
        else if (key == '2') {
            requestCow = true;
            lastRequester = "cow";
            cout << "[Запрос] Коровы хотят выйти!" << endl;
        }
        else if (key == '3') {
            requestGoat = true;
            lastRequester = "goat";
            cout << "[Запрос] Козы хотят выйти!" << endl;
        }
    }
}

int main() {
    setlocale(LC_ALL, "RUS");
    thread inputThread(inputThreadFunc);

    string activePen = "sheep";        // Кто сейчас зелёный
    int greenTime = 0;

    while (true) {
        bool sheepRequested, cowRequested, goatRequested;

        {
            lock_guard<mutex> lock(requestMutex);
            sheepRequested = requestSheep;
            cowRequested = requestCow;
            goatRequested = requestGoat;
        }

        // Фильтруем запросы от уже активного вольера
        if (activePen == "sheep") sheepRequested = false;
        if (activePen == "cow")   cowRequested = false;
        if (activePen == "goat")  goatRequested = false;

        bool anyRequest = sheepRequested || cowRequested || goatRequested;
        bool multipleRequests = (sheepRequested + cowRequested + goatRequested >= 2);

        // ======= ЕСЛИ НИКТО НЕ ПРОСИТ ПЕРЕКЛЮЧЕНИЯ =======
        if (!anyRequest) {
            if (activePen == "sheep") runPhase(GREEN, RED, RED, 1);
            else if (activePen == "cow") runPhase(RED, GREEN, RED, 1);
            else if (activePen == "goat") runPhase(RED, RED, GREEN, 1);
            ++greenTime;
            continue;
        }

        // ======= ДОБИВАЕМ ДО 15 СЕК ЗЕЛЁНОГО =======
        if (greenTime < 15) {
            int remaining = 15 - greenTime;
            if (activePen == "sheep") runPhase(GREEN, RED, RED, remaining);
            else if (activePen == "cow") runPhase(RED, GREEN, RED, remaining);
            else if (activePen == "goat") runPhase(RED, RED, GREEN, remaining);
        }

        // ======= ПОДГОТОВКА К ПЕРЕКЛЮЧЕНИЮ =======
        string nextPen = "";

        if (multipleRequests) {
            if (activePen == "sheep") {
                if (cowRequested) nextPen = "cow";
                else if (goatRequested) nextPen = "goat";
            }
            else if (activePen == "cow") {
                if (sheepRequested) nextPen = "sheep";
                else if (goatRequested) nextPen = "goat";
            }
            else if (activePen == "goat") {
                if (sheepRequested) nextPen = "sheep";
                else if (cowRequested) nextPen = "cow";
            }
        }
        else {
            if (sheepRequested) nextPen = "sheep";
            else if (cowRequested) nextPen = "cow";
            else if (goatRequested) nextPen = "goat";
        }

        // ======= УМНАЯ ЖЁЛТАЯ ФАЗА (только для active и next) =======
        string sheepYellow = RED, cowYellow = RED, goatYellow = RED;
        if (activePen == "sheep" || nextPen == "sheep") sheepYellow = YELLOW;
        if (activePen == "cow" || nextPen == "cow")   cowYellow = YELLOW;
        if (activePen == "goat" || nextPen == "goat")  goatYellow = YELLOW;

        runPhase(sheepYellow, cowYellow, goatYellow, 5);

        // ======= ПЕРЕКЛЮЧЕНИЕ НА nextPen =======
        if (nextPen == "sheep") {
            activePen = "sheep";
            {
                lock_guard<mutex> lock(requestMutex);
                requestSheep = false;
            }
            runPhase(GREEN, RED, RED, 15);
        }
        else if (nextPen == "cow") {
            activePen = "cow";
            {
                lock_guard<mutex> lock(requestMutex);
                requestCow = false;
            }
            runPhase(RED, GREEN, RED, 15);
        }
        else if (nextPen == "goat") {
            activePen = "goat";
            {
                lock_guard<mutex> lock(requestMutex);
                requestGoat = false;
            }
            runPhase(RED, RED, GREEN, 15);
        }

        // ======= ПЕРЕКЛЮЧЕНИЕ НА ПОСЛЕДНЕГО ПРИ МНОЖЕСТВЕННЫХ ЗАПРОСАХ =======
        if (multipleRequests) {
            // Вторая жёлтая фаза: только active и lastRequester
            string secondNext = lastRequester;
            string sheepYellow2 = RED, cowYellow2 = RED, goatYellow2 = RED;

            if (activePen == "sheep" || secondNext == "sheep") sheepYellow2 = YELLOW;
            if (activePen == "cow" || secondNext == "cow")   cowYellow2 = YELLOW;
            if (activePen == "goat" || secondNext == "goat")  goatYellow2 = YELLOW;

            runPhase(sheepYellow2, cowYellow2, goatYellow2, 5);

            // Переключаемся на последнего
            if (lastRequester == "sheep") {
                activePen = "sheep";
                {
                    lock_guard<mutex> lock(requestMutex);
                    requestSheep = false;
                }
                runPhase(GREEN, RED, RED, 15);
            }
            else if (lastRequester == "cow") {
                activePen = "cow";
                {
                    lock_guard<mutex> lock(requestMutex);
                    requestCow = false;
                }
                runPhase(RED, GREEN, RED, 15);
            }
            else if (lastRequester == "goat") {
                activePen = "goat";
                {
                    lock_guard<mutex> lock(requestMutex);
                    requestGoat = false;
                }
                runPhase(RED, RED, GREEN, 15);
            }
        }

        greenTime = 0;
    }

    running = false;
    inputThread.join();
    return 0;
}