#include <iostream>
#include <chrono>
#include <thread>
#include <string>

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

// �������� ������ ��������
string getGateStatus(const string currentLight, const string prevLight) {
    if (currentLight == GREEN) return "�������";
    if (currentLight == YELLOW && prevLight == GREEN) return "�������";
    return "�������";
}

// ������� ������ ��������� ���������� � ��������� ������ c ������ ������ ���������
void printConsol(const string lightSheep, const string lightCow) {

    string gateSheep = getGateStatus(lightSheep, prevLightSheep);
    string gateCow = getGateStatus(lightCow, prevLightCow);

    system("cls");

    cout << "    ������ ������ � �������: " << secondsSinceStart << endl;
    cout << "  --------------------------------" << endl;
    cout << "  |" << "          ��� ����:            |" << endl;
    cout << "  --------------------------------" << endl;
    cout << "  |" << "   ��������   |     " << lightSheep << "    |" << endl;
    cout << "  --------------------------------" << endl;
    cout << "  |" << "   ��������   |     " << gateSheep << "    |" << endl;
    cout << "  --------------------------------" << endl;
    cout << "  |" << "          ��� �����:           |" << endl;
    cout << "  --------------------------------" << endl;
    cout << "  |" << "   ��������   |     " << lightCow << "    |" << endl;
    cout << "  --------------------------------" << endl;
    cout << "  |" << "   ��������   |     " << gateCow << "    |" << endl;
    cout << "  --------------------------------" << endl << endl << endl;
}

// �������, ����������� ���� � �������� �������������
void runPhase(const string lightSheep, const string lightCow, int durationSeconds) {
    for (int i = 0; i < durationSeconds; ++i) {
        printConsol(lightSheep, lightCow);
        this_thread::sleep_for(seconds(1));
        ++secondsSinceStart;
    }

    // ��������� ������� ����� ��� ����������
    prevLightSheep = lightSheep;
    prevLightCow = lightCow;
}


int main() {
    setlocale(LC_ALL, "RUS");
    while (true) {
        runPhase(GREEN, RED, 15);      // ����, ����� �������� ��� ���� �������, ��� ����� - �������
        runPhase(YELLOW, YELLOW, 5);  // ����, ����� ��� ��������� �����
        runPhase(RED, GREEN, 15);     // ����, ����� �������� ��� ���� �������, ��� ����� - �������
        runPhase(YELLOW, YELLOW, 5);  // ����, ����� ��� ��������� �����
    }

    return 0;
}
