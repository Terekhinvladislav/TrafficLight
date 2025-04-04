#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <mutex>
#include <atomic>

using namespace std;
using namespace chrono;

const string GREEN = "��˨���";
const string YELLOW = "ƨ����";
const string RED = "�������";


// ���������� ���������� ��� �������� ��������� ������ c ������ ������ ���������
int secondsSinceStart = 1;

// ���������� ����� ����������
string prevLightSheep = RED;
string prevLightCow = RED;
string prevLightGoat = RED;


bool requestSheep = false;
bool requestCow = false;
bool requestGoat = false;

mutex requestMutex;
atomic<bool> running(true);

// ���������� ���������� ��� ���������� ��������� �� ������
string lastRequester = "";

// �������� ������ ��������
string getGateStatus(const string currentLight, const string prevLight) {
    if (currentLight == GREEN) return "�������";
    if (currentLight == YELLOW && prevLight == GREEN) return "�������";
    return "�������";
}

// ������� ������ ��������� ���������� � ��������� ������ c ������ ������ ���������
void printConsol(const string lightSheep, const string lightCow, const string lightGoat) {

    string gateSheep = getGateStatus(lightSheep, prevLightSheep);
    string gateCow = getGateStatus(lightCow, prevLightCow);
    string gateGoat = getGateStatus(lightGoat, prevLightGoat);

    system("cls");

    cout << "    ������ ������ � �������: " << secondsSinceStart << endl << endl;

    cout << "============================================================" << endl;
    cout <<"  ��������� ��� ���������:" << endl;
    cout <<"  ������� [1] - ������� ������ � ������" << endl;
    cout <<"  ������� [2] - ������� ������ � ��������" << endl;
    cout <<"  ������� [3] - ������� ������ � ������" << endl;
    cout << "============================================================" << endl << endl;


    cout << "  --------------------------------" << endl;
    cout << "  |" << "          ��� ����:            |" << endl;
    cout << "  --------------------------------" << endl;
    cout << "  |" << "   ��������   |     " << lightSheep << "    |" << endl;
    cout << "  --------------------------------" << endl;
    cout << "  |" << "   ��������   |     " << gateSheep << "    |" << endl;
    cout << "  --------------------------------" << endl << endl;
    cout << "  --------------------------------" << endl;
    cout << "  |" << "          ��� �����:           |" << endl;
    cout << "  --------------------------------" << endl;
    cout << "  |" << "   ��������   |     " << lightCow << "    |" << endl;
    cout << "  --------------------------------" << endl;
    cout << "  |" << "   ��������   |     " << gateCow << "    |" << endl;
    cout << "  --------------------------------" << endl << endl;
    cout << "  --------------------------------" << endl;
    cout << "  |" << "          ��� ���:             |" << endl;
    cout << "  --------------------------------" << endl;
    cout << "  |   ��������   |   " << lightGoat << "      |" << endl;
    cout << "  --------------------------------" << endl;
    cout << "  |   ��������   |   " << gateGoat << "      |" << endl;
    cout << "  --------------------------------" << endl << endl;

    cout << "  �������� ������: "
        << (lightSheep == GREEN || lightSheep == YELLOW ? "����" :
            lightCow == GREEN || lightCow == YELLOW ? "������" :
            lightGoat == GREEN || lightGoat == YELLOW ? "����" : "���") << endl;

}

// �������, ����������� ���� � �������� �������������
void runPhase(const string lightSheep, const string lightCow, const string lightGoat, int durationSeconds) {
    for (int i = 0; i < durationSeconds; ++i) {
        printConsol(lightSheep, lightCow, lightGoat);
        this_thread::sleep_for(seconds(1));
        ++secondsSinceStart;
    }

    // ��������� ������� ����� ��� ����������
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
            cout << "[������] ���� ����� �����!" << endl;
        }
        else if (key == '2') {
            requestCow = true;
            lastRequester = "cow";
            cout << "[������] ������ ����� �����!" << endl;
        }
        else if (key == '3') {
            requestGoat = true;
            lastRequester = "goat";
            cout << "[������] ���� ����� �����!" << endl;
        }
    }
}

int main() {
    setlocale(LC_ALL, "RUS");
    thread inputThread(inputThreadFunc);

    string activePen = "sheep";        // ��� ������ ������
    int greenTime = 0;

    while (true) {
        bool sheepRequested, cowRequested, goatRequested;

        {
            lock_guard<mutex> lock(requestMutex);
            sheepRequested = requestSheep;
            cowRequested = requestCow;
            goatRequested = requestGoat;
        }

        // ��������� ������� �� ��� ��������� �������
        if (activePen == "sheep") sheepRequested = false;
        if (activePen == "cow")   cowRequested = false;
        if (activePen == "goat")  goatRequested = false;

        bool anyRequest = sheepRequested || cowRequested || goatRequested;
        bool multipleRequests = (sheepRequested + cowRequested + goatRequested >= 2);

        // ======= ���� ����� �� ������ ������������ =======
        if (!anyRequest) {
            if (activePen == "sheep") runPhase(GREEN, RED, RED, 1);
            else if (activePen == "cow") runPhase(RED, GREEN, RED, 1);
            else if (activePen == "goat") runPhase(RED, RED, GREEN, 1);
            ++greenTime;
            continue;
        }

        // ======= �������� �� 15 ��� ��˨���� =======
        if (greenTime < 15) {
            int remaining = 15 - greenTime;
            if (activePen == "sheep") runPhase(GREEN, RED, RED, remaining);
            else if (activePen == "cow") runPhase(RED, GREEN, RED, remaining);
            else if (activePen == "goat") runPhase(RED, RED, GREEN, remaining);
        }

        // ======= ���������� � ������������ =======
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

        // ======= ����� ƨ���� ���� (������ ��� active � next) =======
        string sheepYellow = RED, cowYellow = RED, goatYellow = RED;
        if (activePen == "sheep" || nextPen == "sheep") sheepYellow = YELLOW;
        if (activePen == "cow" || nextPen == "cow")   cowYellow = YELLOW;
        if (activePen == "goat" || nextPen == "goat")  goatYellow = YELLOW;

        runPhase(sheepYellow, cowYellow, goatYellow, 5);

        // ======= ������������ �� nextPen =======
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

        // ======= ������������ �� ���������� ��� ������������� �������� =======
        if (multipleRequests) {
            // ������ ����� ����: ������ active � lastRequester
            string secondNext = lastRequester;
            string sheepYellow2 = RED, cowYellow2 = RED, goatYellow2 = RED;

            if (activePen == "sheep" || secondNext == "sheep") sheepYellow2 = YELLOW;
            if (activePen == "cow" || secondNext == "cow")   cowYellow2 = YELLOW;
            if (activePen == "goat" || secondNext == "goat")  goatYellow2 = YELLOW;

            runPhase(sheepYellow2, cowYellow2, goatYellow2, 5);

            // ������������� �� ����������
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