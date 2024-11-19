#include <iostream>
#include <vector>
#include <windows.h>

std::vector<short> array;
HANDLE eventWork, eventCount;
CRITICAL_SECTION cs;

DWORD WINAPI workThread(LPVOID lpParam) {
    int interval;
    std::cout << "������� ��������� �������� (��): ";
    std::cin >> interval;

    for (int i = 0; i < array.size(); ++i) {
        if (i < 12) {
            array[i] = i * i;
        }
        else {
            array[i] = 0;
        }
        Sleep(interval);
    }

    EnterCriticalSection(&cs);
    std::cout << "���� � ��. ������ ������� work\n";
    std::cout << "�������� ������: ";
    for (short num : array) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    std::cout << "����� �� ��. ������ ������� work\n";
    LeaveCriticalSection(&cs);

    SetEvent(eventWork);
    SetEvent(eventCount);
    return 0;
}

DWORD WINAPI countElementThread(LPVOID lpParam) {
    EnterCriticalSection(&cs);
    WaitForSingleObject(eventCount, INFINITE);
    WaitForSingleObject(eventWork, INFINITE);
    int count = 0;
    for (short num : array) {
        if (num != 0) {
            ++count;
        }
    }

  
    std::cout << "���� � ��. ������ ������� CountElement\n";
    std::cout << "���������� ��������� ���������: " << count << std::endl;
    std::cout << "����� �� ��. ������ ������� CountElement\n";
    LeaveCriticalSection(&cs);

    return 0;
}

int main() {
    setlocale(LC_ALL, "ru");
    int size;
    std::cout << "������� ������ �������: ";
    std::cin >> size;
    array.resize(size);

    eventWork = CreateEvent(NULL, TRUE, FALSE, NULL);
    eventCount = CreateEvent(NULL, TRUE, FALSE, NULL);
    InitializeCriticalSection(&cs);

    HANDLE hWorkThread = CreateThread(NULL, 0, workThread, NULL, 0, NULL);
    HANDLE hCountElementThread = CreateThread(NULL, 0, countElementThread, NULL, 0, NULL);

    WaitForSingleObject(eventWork, INFINITE);
    
    std::cout << "���� � ��. ������ ������� main\n";
    for (short num : array) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    EnterCriticalSection(&cs);
    std::cout << "����� �� ��. ������ ������� main\n";
    LeaveCriticalSection(&cs);

    WaitForSingleObject(hWorkThread, INFINITE);
    WaitForSingleObject(hCountElementThread, INFINITE);

    CloseHandle(hWorkThread);
    CloseHandle(hCountElementThread);
    CloseHandle(eventWork);
    CloseHandle(eventCount);
    DeleteCriticalSection(&cs);

    return 0;
}
