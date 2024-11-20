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

#include "uart_session.h"

#include <iostream>
#include <iomanip> // for std::setw, std::setfill

namespace sony::olfactory_device {

// Constructor
UartSession::UartSession()
  : uart_handle_(INVALID_HANDLE_VALUE),
    connected_(false),
    t_flag_(false),
    t_wait_(THREAD_SCENT_WAIT),
    t_scent_("") {}

// Destructor
UartSession::~UartSession() {
  if (connected_) {
    Close();
  }
}

bool UartSession::Open(const char* device_id) {
  // Store device_id in a variable named port_num to emphasize that it represents a port number
  const char* port_num = device_id;

  // Convert the port_num string to a wide string (required by CreateFileW)
  std::wstring wide_port_num = std::wstring(port_num, port_num + strlen(port_num));

  // Open the serial port
  std::cout << "[UartSession] device_id: " << device_id << std::endl;
  uart_handle_ = CreateFileW(wide_port_num.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

  if (uart_handle_ == INVALID_HANDLE_VALUE) {
    std::cerr << "[UartSession] Failed to open UART port: " << port_num << std::endl;
    return false;
  }

  // Configure the serial port
  DCB dcb_serial_params = {0};
  dcb_serial_params.DCBlength = sizeof(dcb_serial_params);

  if (!GetCommState(uart_handle_, &dcb_serial_params)) {
    std::cerr << "[UartSession] Error getting serial port state for: " << port_num << std::endl;
    CloseHandle(uart_handle_);
    uart_handle_ = INVALID_HANDLE_VALUE;
    return false;
  }

  // Set serial communication parameters
  dcb_serial_params.BaudRate = CBR_115200;  // Fixed baud rate to 115200
  dcb_serial_params.ByteSize = 8;           // Data size = 8 bits
  dcb_serial_params.StopBits = ONESTOPBIT;  // One stop bit
  dcb_serial_params.Parity = NOPARITY;      // No parity

  if (!SetCommState(uart_handle_, &dcb_serial_params)) {
    std::cerr << "[UartSession] Error setting serial port state for: " << port_num << std::endl;
    CloseHandle(uart_handle_);
    uart_handle_ = INVALID_HANDLE_VALUE;
    return false;
  }

  // Set timeout paramters
  COMMTIMEOUTS timeouts = { 0 };
  timeouts.ReadIntervalTimeout = 0; // milliseconds
  timeouts.ReadTotalTimeoutConstant = 10;
  timeouts.ReadTotalTimeoutMultiplier = 1;

  if (!SetCommTimeouts(uart_handle_, &timeouts)) {
    std::cerr << "[UartSession] Error setting serial port state for: " << port_num << std::endl;
    CloseHandle(uart_handle_);
    uart_handle_ = INVALID_HANDLE_VALUE;
    return false;
  }

  std::cout << "[UartSession] UART initialized successfully for port: " << port_num << " with baud rate: " << dcb_serial_params.BaudRate << std::endl;
  connected_ = true;
  return true;
}

void UartSession::Close() {
  if (connected_) {
    CloseHandle(uart_handle_);
    uart_handle_ = INVALID_HANDLE_VALUE;
    connected_ = false;
    std::cout << "[UartSession] UART connection closed." << std::endl;
  }
}

bool UartSession::IsConnected() const {
  return connected_;
}

bool UartSession::SendData(const std::string& data) {
  if (!connected_) {
    std::cerr << "[UartSession] UART not connected." << std::endl;
    return false;
  }

  // Write data to UART (platform-dependent)
  DWORD bytes_written;
  if (!WriteFile(uart_handle_, data.c_str(), static_cast<DWORD>(data.size()), &bytes_written, nullptr)) {
    std::cerr << "[UartSession] Failed to send data over UART." << std::endl;
    return false;
  }

  std::cout << "[UartSession] Data sent: " << data << std::endl;
  return true;
}

bool UartSession::SendData(unsigned int data) {
  if (!connected_) {
    std::cerr << "[UartSession] UART not connected." << std::endl;
    return false;
  }

   // Write data to UART (platform-dependent)
  DWORD bytes_written;
  unsigned char byte = 0x00;
  unsigned int  mask = 0xFF000000;

  for (int i = 24; i >= 0; i = i - 8) {
    byte = (data & mask) >> i;
    if (!WriteFile(uart_handle_, (LPCVOID)&byte, sizeof(byte), &bytes_written, nullptr)) {
      std::cerr << "[UartSession] Failed to send data over UART." << std::endl;
      return false;
    }
    mask = mask >> 8;
  }
  std::cout << "[UartSession] Data sent: " << std::hex << std::setw(8) << std::setfill('0') << data << std::endl;
  return true;
}

bool UartSession::RecvData(std::string& data) {
  if (!connected_) {
    std::cerr << "[UartSession] UART not connected." << std::endl;
    return false;
  }

  // Read data from UART (platform-dependent)
  char buffer[1024] = { 0 };
  DWORD bytes_read = 0;
  while (true) {
    if (ReadFile(uart_handle_, buffer, sizeof(buffer) - 1, &bytes_read, nullptr)) {
      if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
//      std::cout << "bytes_read: " << bytes_read << std::endl;
        data = buffer;
        std::cout << "[UartSession] Data recv: " << data << std::endl;
        return true;
      }
    } else {
      std::cerr << "[UartSession] Failed to receive data over UART." << std::endl;
      return false;
    }
  }
}

void UartSession::ThreadFunc() {
  std::string result = "";

  while (t_flag_) {
    if (!t_scent_._Equal("")) {
      if (!this->SendData(t_scent_)) {
        std::cerr << "[UartSession] Failed to send." << std::endl;
      }
      if (!this->RecvData(result)) {
        std::cerr << "[UartSession] Failed to receive." << std::endl;
      }
    }

    if (!t_fan_._Equal("")) {
      if (!this->SendData(t_fan_)) {
        std::cerr << "[UartSession] Failed to send." << std::endl;
      }
      if (!this->RecvData(result)) {
        std::cerr << "[UartSession] Failed to receive." << std::endl;
      }
      t_fan_ = "";  // Set "" in a case of FAN.
    }

    std::this_thread::sleep_for(std::chrono::seconds(t_wait_));
  }
  std::cout << "[UartSession] Thread ending..." << std::endl;
}

bool UartSession::StartThreadFunc() {
  if (!connected_) {
    std::cerr << "[UartSession] UART not connected." << std::endl;
    return false;
  }

  t_flag_ = true;
  t_ = std::thread(&UartSession::ThreadFunc, this);
  std::cout << "[UartSession] Thread has started." << std::endl;
  return true;
}

bool UartSession::StopThreadFunc() {
  if (!connected_) {
    std::cerr << "[UartSession] UART not connected." << std::endl;
    return false;
  }

  t_flag_ = false;
  if (t_.joinable()) {
    t_.join();
  }
  std::cout << "[UartSession] Thread has finished." << std::endl;
  return true;
}

bool UartSession::SetScent(const std::string& cmd, long long wait) {
  if (!connected_) {
    std::cerr << "[UartSession] UART not connected." << std::endl;
    return false;
  }

  t_scent_ = cmd;
  t_wait_ = wait;
  return true;
}

bool UartSession::SetFan(const std::string& cmd, long long wait) {
  if (!connected_) {
    std::cerr << "[UartSession] UART not connected." << std::endl;
    return false;
  }

  t_fan_ = cmd;
  t_wait_ = wait;
  return true;
}

bool UartSession::IsScentEmissionAvailable() {
  return true;
}

}  // namespace sony::olfactory_device
