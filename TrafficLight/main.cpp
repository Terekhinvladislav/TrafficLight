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
bool wolvesActive = false;


const string RED_COLOR = "\033[31m";
const string GREEN_COLOR = "\033[32m";
const string YELLOW_COLOR = "\033[33m";
const string GRAY_COLOR = "\033[90m";


mutex requestMutex;
atomic<bool> running(true);

// ���������� ���������� ��� ���������� ��������� �� ������
string lastRequester = "";


string colorText(const string& text, const string& colorCode) {
    return colorCode + text + "\033[0m";
}

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

    // ���������� ��������� ����������
    string coloredLightSheep = colorText(lightSheep, lightSheep == GREEN ? GREEN_COLOR : lightSheep == YELLOW ? YELLOW_COLOR : RED_COLOR);
    string coloredLightCow = colorText(lightCow, lightCow == GREEN ? GREEN_COLOR : lightCow == YELLOW ? YELLOW_COLOR : RED_COLOR);
    string coloredLightGoat = colorText(lightGoat, lightGoat == GREEN ? GREEN_COLOR : lightGoat == YELLOW ? YELLOW_COLOR : RED_COLOR);

    // ���������� ��������� ��������
    string coloredGateSheep = colorText(gateSheep, gateSheep == "�������" ? GREEN_COLOR : RED_COLOR);
    string coloredGateCow = colorText(gateCow, gateCow == "�������" ? GREEN_COLOR : RED_COLOR);
    string coloredGateGoat = colorText(gateGoat, gateGoat == "�������" ? GREEN_COLOR : RED_COLOR);

    // ���������� ��������� ������ �������
    string buttonSheepStatus = colorText(requestSheep ? "������" : "�� ������", requestSheep ? GREEN_COLOR : GRAY_COLOR);
    string buttonCowStatus = colorText(requestCow ? "������" : "�� ������", requestCow ? GREEN_COLOR : GRAY_COLOR);
    string buttonGoatStatus = colorText(requestGoat ? "������" : "�� ������", requestGoat ? GREEN_COLOR : GRAY_COLOR);

    system("cls");

    cout << "    ������ ������ � �������: " << secondsSinceStart << endl << endl;

    cout << "============================================================" << endl;
    cout << colorText("  ��������� ��� ���������:", YELLOW_COLOR) << endl;
    cout << colorText("  ������� [1] - ������� ������ � ������", GREEN_COLOR) << endl;
    cout << colorText("  ������� [2] - ������� ������ � ��������", GREEN_COLOR) << endl;
    cout << colorText("  ������� [3] - ������� ������ � ������", GREEN_COLOR) << endl;
    cout << colorText("  ������� [W] ��� [w] - �����! ������� ��� �������", RED_COLOR) << endl;
    cout << "============================================================" << endl << endl;

    // ����
    cout << "  --------------------------------" << endl;
    cout << "  |" << "          ��� ����:           |" << endl;
    cout << "  --------------------------------" << endl;
    cout << "  |" << "   ��������     | " << coloredLightSheep << "     |" << endl;
    cout << "  --------------------------------" << endl;
    cout << "  |" << "   ��������     | " << coloredGateSheep << "     |" << endl;
    cout << "  --------------------------------" << endl;
    cout << "  |" << " ������ ������� | " << buttonSheepStatus << "   |" << endl;
    cout << "  --------------------------------" << endl << endl;

    // ������
    cout << "  --------------------------------" << endl;
    cout << "  |" << "          ��� �����:          |" << endl;
    cout << "  --------------------------------" << endl;
    cout << "  |" << "   ��������     | " << coloredLightCow << "     |" << endl;
    cout << "  --------------------------------" << endl;
    cout << "  |" << "   ��������     | " << coloredGateCow << "     |" << endl;
    cout << "  --------------------------------" << endl;
    cout << "  |" << " ������ ������� | " << buttonCowStatus << "   |" << endl;
    cout << "  --------------------------------" << endl << endl;

    // ����
    cout << "  --------------------------------" << endl;
    cout << "  |" << "          ��� ���:            |" << endl;
    cout << "  --------------------------------" << endl;
    cout << "  |   ��������      | " << coloredLightGoat << "    |" << endl;
    cout << "  --------------------------------" << endl;
    cout << "  |   ��������      | " << coloredGateGoat << "    |" << endl;
    cout << "  --------------------------------" << endl;
    cout << "  | ������ �������  | " << buttonGoatStatus << "  |" << endl;
    cout << "  --------------------------------" << endl << endl;

    // ������� �������� ������
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
            if (wolvesActive) {
                cout << "[������������] ������ ������������ � ������� '�����' �������.\n";
                continue;
            }
            requestSheep = true;
            lastRequester = "sheep";
            cout << "[������] ���� ����� �����!" << endl;
        }
        else if (key == '2') {
            if (wolvesActive) {
                cout << "[������������] ������ ������������ � ������� '�����' �������.\n";
                continue;
            }
            requestCow = true;
            lastRequester = "cow";
            cout << "[������] ������ ����� �����!" << endl;
        }
        else if (key == '3') {
            if (wolvesActive) {
                cout << "[������������] ������ ������������ � ������� '�����' �������.\n";
                continue;
            }
            requestGoat = true;
            lastRequester = "goat";
            cout << "[������] ���� ����� �����!" << endl;
        }
        else if (key == 'w' || key == 'W') {
            wolvesActive = !wolvesActive;
            cout << (wolvesActive ? "[�������] �����! ��� ������� �����������." : "[������ �������] ����� ����� ������.") << endl;
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

        if (wolvesActive) {
            // ������� ��������� ������� �� ����� (��������� � �������)
            if (activePen == "sheep") {
                runPhase(YELLOW, RED, RED, 5);
            }
            else if (activePen == "cow") {
                runPhase(RED, YELLOW, RED, 5);
            }
            else if (activePen == "goat") {
                runPhase(RED, RED, YELLOW, 5);
            }

            // ��� ������� ������� �� ����� �������
            while (wolvesActive) {
                runPhase(RED, RED, RED, 1);
            }

            // ����� ������ ������� � ���������� ������������ ������ (����)
            activePen = "sheep";
            greenTime = 0;
            continue;
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