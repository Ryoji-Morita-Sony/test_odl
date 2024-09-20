/**
 * Sony CONFIDENTIAL
 *
 * Copyright 2024 Sony Group Corporation
 *
 * DO NOT COPY AND/OR REDISTRIBUTE WITHOUT PERMISSION.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

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
