#include <stdio.h>
#include <Windows.h>
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

    while(1) {
        if(ReadFile(serial_handle, buff, 1, &read, &ol) != 0) {
            k = (uint8_t)(*buff);
            printf("%d:", i);
            for(int l = 0; l < 8; ++l) {
                bool val = (k & bits[l]) != 0;
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