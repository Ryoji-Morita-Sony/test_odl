// Copyright (c) 2024 Sony Corporation
//
// This source code is unlicensed and all use is prohibited without the express permission of the copyright
// holder.
//
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.

#include "uart_initializer.h"

#include <stdio.h>

namespace sony::olfactory_device {

UARTInitializer::UARTInitializer() {
  // UARTÇÃèâä˙âª
  hSerial = CreateFileW(L"COM3", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

  if (hSerial == INVALID_HANDLE_VALUE) {
    printf("Failed to open UART port.\n");
  } else {
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
      hSerial = INVALID_HANDLE_VALUE;
    } else {
      printf("UART initialized successfully.\n");
    }
  }
}

bool UARTInitializer::SendUARTCommand(const char* command) {
  if (hSerial == INVALID_HANDLE_VALUE) {
    printf("UART port is not opened.\n");
    return false;
  }

  DWORD bytesWritten;
  if (!WriteFile(hSerial, command, strlen(command), &bytesWritten, NULL)) {
    printf("Failed to send command over UART.\n");
    return false;
  }

  printf("Command sent: %s\n", command);
  return true;
}

}  // namespace sony::olfactory_device
