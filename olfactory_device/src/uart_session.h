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

#pragma once
#include "device_session_if.h"
#include <string>
#include <windows.h>
#include <thread>
#include <atomic>

namespace sony::olfactory_device {

/**
 * @brief UartSession class implements the DeviceSessionIF interface for UART communication.
 *
 * This class manages the UART session, providing methods to open, close, check connection status,
 * and send data over a UART connection.
 */
class UartSession : public DeviceSessionIF {
 private:
  HANDLE uart_handle_;  // Handle to the UART connection
  bool connected_;      // Connection status

  std::atomic<bool> t_flag_;      // Flag for thread loop.
  long long         t_wait_;      // wait time of thread loop, milliseconds.
  std::thread       thread_;      // Thread parameter.
  std::atomic<bool> s_dataFlag_;  // Flag if data(string type) is sent in thread loop.
  std::string       s_data_;      // Data(string type) to send in thread loop.
  std::atomic<bool> ui_dataFlag_; // Flag if data(ui) is sent in thread loop.
  unsigned int      ui_data_;     // Data(ui) to send in thread loop.

 public:
  UartSession();
  ~UartSession() override;

  /**
   * @brief Opens a UART session with the specified device.
   *
   * @param device_id The identifier of the UART device (e.g., COM port name).
   * @return Returns true if the connection was successfully established, false otherwise.
   */
  bool Open(const char* device_id) override;

  /**
   * @brief Closes the UART session.
   */
  void Close() override;

  /**
   * @brief Checks if the UART session is connected.
   *
   * @return Returns true if the session is connected, false otherwise.
   */
  bool IsConnected() const override;

  /**
   * @brief Sends data over the UART connection.
   *
   * @param data The data to send over the UART.
   * @return Returns true if the data was successfully sent, false otherwise.
   */
  bool SendData(const std::string& data) override;

  // Add any other necessary interface methods here
  bool SendData(unsigned int data) override;

  /**
   * @brief Received data over the UART connection.
   *
   * @param data The data to receive over the UART.
   * @return Returns true if the data was successfully sent, false otherwise.
   */
  bool RecvData(std::string& data) override;

 private:
  /**
   * @brief Thread function.
   */
  void ThreadFunction() override;

 public:
  /**
   * @brief Start a thread.
   *
   * @param wait milliseconds waiting in thread loop.
   */
  void StartThread(long long wait) override;

/**
   * @brief Stop a thread.
   */
  void StopThread() override;

/**
   * @brief Set a data.
   *
   * @param data A data to set in thread loop.
   */
  void SetData(const std::string& data) override;
  void SetData(unsigned int data) override;

};

}  // namespace sony::olfactory_device
