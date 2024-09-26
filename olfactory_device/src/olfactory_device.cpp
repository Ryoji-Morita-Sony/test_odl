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

#include "olfactory_device.h"

#include "uart_client.h"

#include <iostream>
#include <unordered_map>

namespace sony::olfactory_device {

// Map to manage UARTClient instances by device_id
static std::unordered_map<std::string, UARTClient> uartClients;

OLFACTORY_DEVICE_API OdResult sony_odStartSession(const char* device_id) {
  std::string device(device_id);

  // Check if a session is already active for the given device_id
  if (uartClients.find(device) != uartClients.end() && uartClients[device].IsConnected()) {
    std::cerr << "Session is already active on port: " << device_id << "\n";
    return OdResult::ERROR_UNKNOWN;
  }

  // Add and construct the UARTClient directly in the map
  auto result = uartClients.emplace(std::make_pair(device, UARTClient()));
  UARTClient& uartClient = result.first->second;

  // Open the UART port for the newly created UARTClient
  if (!uartClient.Open(device_id)) {
    std::cerr << "Failed to open UART connection on port: " << device_id << "\n";
    uartClients.erase(device);  // Remove the client if opening the port failed
    return OdResult::ERROR_UNKNOWN;
  }

  std::cout << "Session started on port: " << device_id << "\n";
  return OdResult::SUCCESS;
}

OLFACTORY_DEVICE_API OdResult sony_odEndSession(const char* device_id) {
  std::string device(device_id);

  // Check if a session is active for the given device_id
  if (uartClients.find(device) == uartClients.end() || !uartClients[device].IsConnected()) {
    std::cerr << "No active session on port: " << device_id << "\n";
    return OdResult::ERROR_UNKNOWN;
  }

  // Close the UART port and remove the UARTClient instance
  uartClients[device].Close();
  uartClients.erase(device);

  std::cout << "Session ended on port: " << device_id << "\n";
  return OdResult::SUCCESS;
}

OLFACTORY_DEVICE_API OdResult sony_odSetScentOrientation(const char* device_id, float yaw, float pitch) {
  std::string device(device_id);

  // Check if session is active for the given device_id
  if (uartClients.find(device) == uartClients.end() || !uartClients[device].IsConnected()) {
    std::cerr << "No active session on port: " << device_id << ". Start a session first.\n";
    return OdResult::ERROR_UNKNOWN;
  }

  // Send the command to set scent orientation over UART
  std::string command = "SET_ORIENTATION " + std::to_string(yaw) + " " + std::to_string(pitch) + "\n";
  if (!uartClients[device].SendData(command)) {
    std::cerr << "Failed to send set orientation command on port: " << device_id << "\n";
    return OdResult::ERROR_UNKNOWN;
  }

  std::cout << "Scent orientation set on port: " << device_id << ", Yaw: " << yaw << ", Pitch: " << pitch
            << "\n";
  return OdResult::SUCCESS;
}

OLFACTORY_DEVICE_API OdResult sony_odStartScentEmission(const char* device_id, const char* scent_name,
                                                        float level) {
  std::string device(device_id);

  // Check if session is active for the given device_id
  if (uartClients.find(device) == uartClients.end() || !uartClients[device].IsConnected()) {
    std::cerr << "No active session on port: " << device_id << ". Start a session first.\n";
    return OdResult::ERROR_UNKNOWN;
  }

  // Send the command to start scent emission over UART
  std::string command = "START_SCENT " + std::string(scent_name) + " " + std::to_string(level) + "\n";
  if (!uartClients[device].SendData(command)) {
    std::cerr << "Failed to send scent emission command on port: " << device_id << "\n";
    return OdResult::ERROR_UNKNOWN;
  }

  std::cout << "Scent emission started on port: " << device_id << ", Scent: " << scent_name
            << ", Level: " << level << "\n";
  return OdResult::SUCCESS;
}

OLFACTORY_DEVICE_API OdResult sony_odStopScentEmission(const char* device_id) {
  std::string device(device_id);

  // Check if session is active for the given device_id
  if (uartClients.find(device) == uartClients.end() || !uartClients[device].IsConnected()) {
    std::cerr << "No active session on port: " << device_id << ". Start a session first.\n";
    return OdResult::ERROR_UNKNOWN;
  }

  // Send the command to stop scent emission over UART
  std::string command = "STOP_SCENT\n";
  if (!uartClients[device].SendData(command)) {
    std::cerr << "Failed to send stop scent emission command on port: " << device_id << "\n";
    return OdResult::ERROR_UNKNOWN;
  }

  std::cout << "Scent emission stopped on port: " << device_id << "\n";
  return OdResult::SUCCESS;
}

}  // namespace sony::olfactory_device
