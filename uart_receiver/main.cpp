// Copyright (c) 2024 Sony Corporation
//
// This source code is unlicensed and all use is prohibited without the express permission of the copyright
// holder.
//
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.

#include <stdio.h>
#include <windows.h>

int main() {
  // COMポートを開く（受信側）
  HANDLE hSerial =
      CreateFileW(L"COM4", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

  if (hSerial == INVALID_HANDLE_VALUE) {
    printf("Failed to open COM4\n");
    return 1;
  }

  // シリアルポートの設定
  DCB dcbSerialParams = {0};
  dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
  GetCommState(hSerial, &dcbSerialParams);

  dcbSerialParams.BaudRate = CBR_9600;
  dcbSerialParams.ByteSize = 8;
  dcbSerialParams.StopBits = ONESTOPBIT;
  dcbSerialParams.Parity = NOPARITY;

  if (!SetCommState(hSerial, &dcbSerialParams)) {
    printf("Error setting serial port state.\n");
    CloseHandle(hSerial);
    return 1;
  }

  // タイムアウトの設定
  COMMTIMEOUTS timeouts = {0};
  timeouts.ReadIntervalTimeout = 50;
  timeouts.ReadTotalTimeoutConstant = 50;
  timeouts.ReadTotalTimeoutMultiplier = 10;
  SetCommTimeouts(hSerial, &timeouts);

  // 受信バッファ
  char szBuff[256] = {0};
  DWORD dwBytesRead = 0;

  // データ受信ループ
  printf("Waiting for data...\n");
  while (true) {
    if (ReadFile(hSerial, szBuff, sizeof(szBuff) - 1, &dwBytesRead, NULL)) {
      if (dwBytesRead > 0) {
        szBuff[dwBytesRead] = '\0';  // Null終端
        printf("Received: %s\n", szBuff);
      }
    } else {
      printf("Error reading from COM4.\n");
      break;
    }
  }

  // シリアルポートを閉じる
  CloseHandle(hSerial);
  return 0;
}
