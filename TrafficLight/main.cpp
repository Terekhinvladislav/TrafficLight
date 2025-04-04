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

// ������� ������ ��������� ���������� � ��������� ������ c ������ ������ ���������
void printConsol(const string light1, const string light2) {

    cout << "    ������ ������ � �������: " << secondsSinceStart << endl;
    cout << "  --------------------------------" << endl;
    cout << "  |" << "         ��� ����:            |" << endl;
    cout << "  --------------------------------" << endl;
    cout << "  |" << "   ��������   |     " << light1 << "    |" << endl;
    cout << "  --------------------------------" << endl;
    cout << "  |" << "         ���� �����:           |" << endl;
    cout << "  --------------------------------" << endl;
    cout << "  |" << "   ��������   |     " << light2 << "    |" << endl;
    cout << "  --------------------------------" << endl << endl << endl;
}

// �������, ����������� ���� � �������� �������������
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
        runPhase(GREEN, RED, 15);      // ����, ����� �������� ��� ���� �������, ��� ����� - �������
        runPhase(YELLOW, YELLOW, 5);  // ����, ����� ��� ��������� �����
        runPhase(RED, GREEN, 15);     // ����, ����� �������� ��� ���� �������, ��� ����� - �������
        runPhase(YELLOW, YELLOW, 5);  // ����, ����� ��� ��������� �����
    }

    return 0;
}
