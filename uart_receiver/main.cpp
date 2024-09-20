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
