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

#include <iostream>
#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"

#define THREAD_WAIT (200)
#define OSC_PORT (7000)

namespace sony::olfactory_device {

/**
 * @brief OscSession class implements the DeviceSessionIF interface for OSC communication.
 *
 * This class manages the OSC session, providing methods to open, close, check connection status,
 * and send data over a OSC connection.
 */
class OscSession : public DeviceSessionIF {
 private:
  std::string       osc_ip_;      // OSC IP address
  int               osc_port_;    // OSC port
  bool connected_;        // Connection status

 public:
  OscSession();
  ~OscSession() override;

  /**
   * @brief Opens a OSC session with the specified device.
   *
   * @param device_id The identifier of the OSC device (e.g., IP, port name).
   * @return Returns true if the connection was successfully established, false otherwise.
   */
  bool Open(const char* device_id) override;

  /**
   * @brief Closes the OSC session.
   */
  void Close() override;

  /**
   * @brief Checks if the OSC session is connected.
   *
   * @return Returns true if the session is connected, false otherwise.
   */
  bool IsConnected() const override;

  /**
   * @brief Sends data over the OSC connection.
   *
   * @param data The data to send over the OSC.
   * @return Returns true if the data was successfully sent, false otherwise.
   */
  bool SendData(const std::string& data) override;

  /**
   * @brief Received data over the OSC connection.
   *
   * @param data The data to receive over the OSC.
   * @return Returns true if the data was successfully sent, false otherwise.
   */
  bool RecvData(std::string& data) override;

  /**
   * @brief Check if scent emission is available for the specified device.
   *
   * @return Returns true if the availability check is performed successfully, otherwise false.
   */
  bool IsScentEmissionAvailable() override;
};

}  // namespace sony::olfactory_device
