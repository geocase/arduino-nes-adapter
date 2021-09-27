#include <stdio.h>
#include <Windows.h>
#include <winuser.h>
#include <inttypes.h>
#include <stdbool.h>

int main() {
    HANDLE serial_handle = CreateFileA("\\\\.\\COM3", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    LPDWORD flags;
    

    DCB serial_params = {0};
    serial_params.DCBlength = sizeof(serial_params);
    GetCommState(serial_handle, &serial_params);
    serial_params.BaudRate = CBR_9600;
    serial_params.ByteSize = DATABITS_8;
    serial_params.StopBits = ONESTOPBIT;
    serial_params.Parity = NOPARITY;

    COMMTIMEOUTS timeout = {0};
    timeout.ReadIntervalTimeout = 1;
    timeout.ReadTotalTimeoutConstant = 1;
    timeout.ReadTotalTimeoutMultiplier = 1;

    uint64_t i = 0;
    LPVOID* buff = malloc(sizeof(uint8_t));
    DWORD read = 1;
    LPTSTR buffer;
    DWORD cchBufferLength;
    OVERLAPPED ol = {0};

    FlushFileBuffers(serial_handle);
    PurgeComm(serial_handle, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);

    uint8_t k = 0;

    uint8_t bits[] = {1, 2, 4, 8, 16, 32, 64, 128};
    // h, j, u, i, w, s, a, d
    WORD keys[] = {0x48, 0x4a, 0x55, 0x49, 0x57, 0x53, 0x41, 0x44};
    bool key_pressed[] = {false, false, false, false, false, false, false, false};

    INPUT out = {0};
    out.type = INPUT_KEYBOARD;
    out.ki.wVk = 49;
    out.ki.wScan = 0;
    out.ki.time = 0;
    out.ki.dwExtraInfo = 0;
    out.ki.dwFlags = 0;

    while(1) {
        if(ReadFile(serial_handle, buff, 1, &read, &ol) != 0) {
            k = (uint8_t)(*buff);
            printf("%d:", i);
            for(int l = 0; l < 8; ++l) {
                bool val = (k & bits[l]) != 0;
                out.ki.wScan = MapVirtualKey(keys[l], 0);
                if(val && (!key_pressed[l])) {
                    out.ki.dwFlags = KEYEVENTF_SCANCODE;
                    SendInput(1, &out, sizeof(INPUT));
                    key_pressed[l] = true;
                } else if(!val && key_pressed[l]) {
                    out.ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_SCANCODE;
                    SendInput(1, &out, sizeof(INPUT));
                    key_pressed[l] = false;
                }
                printf("%d\t", val);                    
            }
            printf("\n");

        } else {
            printf("%ld\n", GetLastError());
        }
        ++i;
    }

    return 0;
}