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
#include "device_session_if.h"
#include "uart_session.h"
#include "stub_session.h"

#include <iostream>
#include <unordered_map>

namespace sony::olfactory_device {

// Uncomment to use the StubSession for testing
//#define USE_STUB_SESSION

#ifdef USE_STUB_SESSION
using SessionType = StubSession;
#else
using SessionType = UartSession;
#endif

// Map to manage DeviceSessionIF instances by device_id
static std::unordered_map<std::string, std::unique_ptr<DeviceSessionIF>> device_sessions;

OLFACTORY_DEVICE_API OdResult sony_odStartSession(const char* device_id) {
  std::string device(device_id);

  // Check if a session is already active for the given device_id
  if (device_sessions.find(device) != device_sessions.end() && device_sessions[device]->IsConnected()) {
    std::cerr << "Session is already active on port: " << device_id << "\n";
    return OdResult::ERROR_UNKNOWN;
  }

  // Emplace the new SessionType (either StubSession or UartSession)
  device_sessions.emplace(device, std::make_unique<SessionType>());

  // Open the session for the newly created session instance
  if (!device_sessions[device]->Open(device_id)) {
    std::cerr << "Failed to open connection on port: " << device_id << "\n";
    device_sessions.erase(device);  // Remove if failed
    return OdResult::ERROR_UNKNOWN;
  }

  return OdResult::SUCCESS;
}

OLFACTORY_DEVICE_API OdResult sony_odEndSession(const char* device_id) {
  std::string device(device_id);

  // Check if a session is active for the given device_id
  if (device_sessions.find(device) == device_sessions.end() || !device_sessions[device]->IsConnected()) {
    std::cerr << "No active session on port: " << device_id << "\n";
    return OdResult::ERROR_UNKNOWN;
  }

  // Close the session and remove it from the map
  device_sessions[device]->Close();
  device_sessions.erase(device);

  return OdResult::SUCCESS;
}

OLFACTORY_DEVICE_API OdResult sony_odSetScentOrientation(const char* device_id, float yaw, float pitch) {
  std::string device(device_id);

  // Check if a session is active for the given device_id
  if (device_sessions.find(device) == device_sessions.end() || !device_sessions[device]->IsConnected()) {
    std::cerr << "No active session on port: " << device_id << ". Start a session first.\n";
    return OdResult::ERROR_UNKNOWN;
  }

  // Send the command to set scent orientation
  std::string command = "SET_ORIENTATION " + std::to_string(yaw) + " " + std::to_string(pitch) + "\n";
  if (!device_sessions[device]->SendData(command)) {
    std::cerr << "Failed to send set orientation command on port: " << device_id << "\n";
    return OdResult::ERROR_UNKNOWN;
  }

  return OdResult::SUCCESS;
}

OLFACTORY_DEVICE_API OdResult sony_odStartScentEmission(const char* device_id, const char* scent_name,
                                                        float level) {
  std::string device(device_id);

  // Check if a session is active for the given device_id
  if (device_sessions.find(device) == device_sessions.end() || !device_sessions[device]->IsConnected()) {
    std::cerr << "No active session on port: " << device_id << ". Start a session first.\n";
    return OdResult::ERROR_UNKNOWN;
  }

  // Send the command to start scent emission
  std::string command = "START_SCENT " + std::string(scent_name) + " " + std::to_string(level) + "\n";
  if (!device_sessions[device]->SendData(command)) {
    std::cerr << "Failed to send scent emission command on port: " << device_id << "\n";
    return OdResult::ERROR_UNKNOWN;
  }

  return OdResult::SUCCESS;
}

OLFACTORY_DEVICE_API OdResult sony_odStopScentEmission(const char* device_id) {
  std::string device(device_id);

  // Check if a session is active for the given device_id
  if (device_sessions.find(device) == device_sessions.end() || !device_sessions[device]->IsConnected()) {
    std::cerr << "No active session on port: " << device_id << ". Start a session first.\n";
    return OdResult::ERROR_UNKNOWN;
  }

  // Send the command to stop scent emission
  std::string command = "STOP_SCENT\n";
  if (!device_sessions[device]->SendData(command)) {
    std::cerr << "Failed to send stop scent emission command on port: " << device_id << "\n";
    return OdResult::ERROR_UNKNOWN;
  }

  return OdResult::SUCCESS;
}

}  // namespace sony::olfactory_device
