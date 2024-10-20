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

namespace sony::olfactory_device {

/**
 * @brief StubSession class implements the DeviceSessionIF interface for testing and debugging.
 *
 * This class simulates a device session by logging the operations and arguments received.
 * It provides insight into what actions would be taken if this were a real session.
 */
class StubSession : public DeviceSessionIF {
 private:
  bool connected_;  // Simulated connection status

 public:
  StubSession();
  ~StubSession() override;

  /**
   * @brief Simulates opening a session with the specified device.
   *
   * @param device_id The identifier of the UART device (e.g., COM port name).
   * @return Always returns true to simulate a successful connection.
   */
  bool Open(const char* device_id) override;

  /**
   * @brief Simulates closing the session.
   */
  void Close() override;

  /**
   * @brief Simulates checking if the session is connected.
   *
   * @return Returns true if the session is connected (simulated).
   */
  bool IsConnected() const override;

  /**
   * @brief Simulates sending data over the session.
   *
   * @param data The data that would be sent over the session.
   * @return Always returns true to simulate successful data transmission.
   */
  bool SendData(const std::string& data) override;

  // Add any other necessary interface methods here
  bool SendData(unsigned int data) override;

  /**
   * @brief Simulates received data over the session.
   *
   * @param data The data that would be received over the session.
   * @return Always returns true to simulate successful data transmission.
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
   * @return Returns true if the session is connected (simulated).
   */
  bool StartThreadFunc() override;

  /**
   * @brief Stop a thread.
   *
   * @return Returns true if the session is connected (simulated).
   */
  bool StopThreadFunc() override;

  /**
   * @brief Set a scent command like "release(4, 10)".
   *
   * @param cmd Scent command.
   * @param wait Seconds waiting in thread loop.
   * @return Returns true if the session is connected (simulated).
   */
  bool SetScent(const std::string& cmd, long long wait) override;

  /**
   * @brief Set a fan command like "fan(1, 50)".
   *
   * @param cmd Fan command.
   * @param wait Seconds waiting in thread loop.
   * @return Returns true if the session is connected (simulated).
   */
  bool SetFan(const std::string& cmd, long long wait) override;

};

}  // namespace sony::olfactory_device
