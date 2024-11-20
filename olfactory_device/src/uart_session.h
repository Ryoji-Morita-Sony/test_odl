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
#include <queue>

#define THREAD_WAIT (200)

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

  std::thread       t_;       // Thread parameter.
  std::atomic<bool> t_flag_;  // Flag for thread loop.
  long long         t_wait_;  // Time waiting in thread loop, milliseconds.
  std::string       t_cmd_;   // A command to send in thread loop.

  std::string       scent[4]; // scent devices

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
   * @return Returns true if the data was successfully received, false otherwise.
   */
  bool RecvData(std::string& data) override;

 private:
  /**
   * @brief Thread function.
   */
  void ThreadFunc() override;

 public:
  /**
   * @brief Start a thread.
   *
   * @return Returns true if the session is connected, false otherwise.
   */
  bool StartThreadFunc() override;

  /**
   * @brief Stop a thread.
   *
   * @return Returns true if the session is connected, false otherwise.
   */
  bool StopThreadFunc() override;

  /**
   * @brief Send a command like "release(0, 10)", "fan(1, 50)", "motor(1, 50)".
   *
   * @param cmd A command.
   * @param wait Milliseconds waiting in thread loop.
   * @return Returns true if the session is connected, false otherwise.
   */
  bool SendCmd(const std::string& cmd, long long wait) override;

  /**
   * @brief Set ID and scent-name like "rose", "lavender".
   *
   * @param id ID of SMA device. (0 ~ 3)
   * @param name Name of scent.
   * @return Returns true if it succeeded, false otherwise.
   */
  bool SetScent(unsigned int id, const std::string& name) override;

  /**
   * @brief Get ID from scent-name.
   *
   * @param name Name of scent.
   * @return Returns id (0 ~ 3), -1 if nothing.
   */
  int GetScent(const std::string& name) override;

};

}  // namespace sony::olfactory_device
