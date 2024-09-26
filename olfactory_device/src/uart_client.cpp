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

#include "uart_client.h"

#include <windows.h>

#include <iostream>

// Constructor
UARTClient::UARTClient() : uartHandle_(INVALID_HANDLE_VALUE), connected_(false) {}

// Destructor
UARTClient::~UARTClient() {
  if (connected_) {
    Close();
  }
}

// Open UART connection with a fixed baud rate of CBR_9600
bool UARTClient::Open(const std::string& portName) {
  // Convert the portName string to a wide string (required by CreateFileW)
  std::wstring widePortName = std::wstring(portName.begin(), portName.end());

  // Open the serial port
  uartHandle_ = CreateFileW(widePortName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING,
                            FILE_ATTRIBUTE_NORMAL, 0);

  if (uartHandle_ == INVALID_HANDLE_VALUE) {
    std::cerr << "Failed to open UART port: " << portName << std::endl;
    return false;
  }

  // Configure the serial port
  DCB dcbSerialParams = {0};
  dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

  if (!GetCommState(uartHandle_, &dcbSerialParams)) {
    std::cerr << "Error getting serial port state for: " << portName << std::endl;
    CloseHandle(uartHandle_);
    uartHandle_ = INVALID_HANDLE_VALUE;
    return false;
  }

  // Set serial communication parameters
  dcbSerialParams.BaudRate = CBR_9600;    // Fixed baud rate to 9600
  dcbSerialParams.ByteSize = 8;           // Data size = 8 bits
  dcbSerialParams.StopBits = ONESTOPBIT;  // One stop bit
  dcbSerialParams.Parity = NOPARITY;      // No parity

  if (!SetCommState(uartHandle_, &dcbSerialParams)) {
    std::cerr << "Error setting serial port state for: " << portName << std::endl;
    CloseHandle(uartHandle_);
    uartHandle_ = INVALID_HANDLE_VALUE;
    return false;
  }

  std::cout << "UART initialized successfully for port: " << portName << " with baud rate: 9600" << std::endl;
  connected_ = true;
  return true;
}

// Close UART connection
void UARTClient::Close() {
  if (connected_) {
    CloseHandle(uartHandle_);
    uartHandle_ = INVALID_HANDLE_VALUE;
    connected_ = false;
    std::cout << "UART connection closed." << std::endl;
  }
}

// Send data over UART
bool UARTClient::SendData(const std::string& data) {
  if (!connected_) {
    std::cerr << "UART not connected." << std::endl;
    return false;
  }

  // Write data to UART (platform-dependent)
  WriteFile(uartHandle_, data.c_str(), data.size(), nullptr, nullptr);

  std::cout << "Data sent: " << data << std::endl;
  return true;
}

// Check if UART is connected
bool UARTClient::IsConnected() const { return connected_; }
