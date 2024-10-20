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
#include <string>

namespace sony::olfactory_device {

class DeviceSessionIF {
 public:
  virtual ~DeviceSessionIF() = default;

  /**
   * @brief Opens a connection to the specified device.
   *
   * This function attempts to open a session with the device identified by the provided device_id.
   *
   * @param device_id The identifier of the device to connect to (e.g., a COM port or a stub device).
   * @return Returns true if the connection was successfully established, false otherwise.
   */
  virtual bool Open(const char* device_id) = 0;

  /**
   * @brief Closes the connection to the device.
   *
   * This function closes the session with the connected device. It should be called when the device is
   * no longer needed or when shutting down the application.
   */
  virtual void Close() = 0;

  /**
   * @brief Checks if a connection to the device is currently active.
   *
   * This function returns whether a session to the device is currently open and active.
   *
   * @return Returns true if a session is active, false otherwise.
   */
  virtual bool IsConnected() const = 0;

  /**
   * @brief Sends data to the connected device.
   *
   * This function sends the specified data to the device over the active connection.
   * The device must be connected before calling this function.
   *
   * @param data A reference to the string containing the data to send.
   * @return Returns true if the data was successfully sent, false otherwise.
   */
  virtual bool SendData(const std::string& data) = 0;

  // Add any other necessary interface methods here
  virtual bool SendData(unsigned int data) = 0;

  /**
   * @brief Receives data from the connected device.
   *
   * This function receives the specified data from the device over the active connection.
   * The device must be connected before calling this function.
   *
   * @param data A reference to the string containing the data to receive.
   * @return Returns true if the data was successfully sent, false otherwise.
   */
  virtual bool RecvData(std::string& data) = 0;

 private:
  /**
   * @brief Thread function.
   */
  virtual void ThreadFunc() = 0;

 public:
  /**
   * @brief Start a thread.
   *
   * @return Returns true if it succeeded, false otherwise.
   */
  virtual bool StartThreadFunc() = 0;

  /**
   * @brief Stop a thread.
   *
   * @return Returns true if it succeeded, false otherwise.
   */
  virtual bool StopThreadFunc() = 0;

  /**
   * @brief Set a scent command like "release(4, 10)".
   *
   * @param cmd Scent command.
   * @param wait Seconds waiting in thread loop.
   * @return Returns true if it succeeded, false otherwise.
   */
  virtual bool SetScent(const std::string& cmd, long long wait) = 0;

  /**
   * @brief Set a fan command like "fan(1, 50)".
   *
   * @param cmd Fan command.
   * @param wait Seconds waiting in thread loop.
   * @return Returns true if it succeeded, false otherwise.
   */
  virtual bool SetFan(const std::string& cmd, long long wait) = 0;

};

}  // namespace sony::olfactory_device
