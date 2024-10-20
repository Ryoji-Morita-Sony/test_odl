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
#include "osc_session.h"

#include <iostream>
#include <unordered_map>
#include <iomanip> // for std::setw, std::setfill
#include <string>

namespace sony::olfactory_device {

// Uncomment to use the StubSession for testing
//#define USE_STUB_SESSION
//#define USE_UART_SESSION

#ifdef USE_STUB_SESSION
using SessionType = StubSession;
#elif defined(USE_UART_SESSION)
using SessionType = UartSession;
#else
using SessionType = OscSession;
#endif

// Map to manage DeviceSessionIF instances by device_id
static std::unordered_map<std::string, std::unique_ptr<DeviceSessionIF>> device_sessions;

OLFACTORY_DEVICE_API OdResult sony_odStartSession(const char* device_id) {
  std::string device(device_id);

  // Check if a session is already active for the given device_id
  if (device_sessions.find(device) != device_sessions.end() && device_sessions[device]->IsConnected()) {
    std::cerr << "Session is already active on port: " << device_id << std::endl;
    return OdResult::ERROR_UNKNOWN;
  }

  // Emplace the new SessionType (either StubSession or UartSession)
  device_sessions.emplace(device, std::make_unique<SessionType>());

  // Open the session for the newly created session instance
  if (!device_sessions[device]->Open(device_id)) {
    std::cerr << "Failed to open connection on port: " << device_id << std::endl;
    device_sessions.erase(device);  // Remove if failed
    return OdResult::ERROR_UNKNOWN;
  }

  if (!device_sessions[device]->StartThreadFunc()) {
    std::cerr << "Failed to start tread" << std::endl;
    return OdResult::ERROR_UNKNOWN;
  }

//  if (!device_sessions[device]->SetFan("fan(1, 30)", 1)) {
//    std::cerr << "Failed to set FAN." << std::endl;
//    return OdResult::ERROR_UNKNOWN;
//  }

  return OdResult::SUCCESS;
}

OLFACTORY_DEVICE_API OdResult sony_odEndSession(const char* device_id) {
  std::string device(device_id);

  // Check if a session is active for the given device_id
  if (device_sessions.find(device) == device_sessions.end() || !device_sessions[device]->IsConnected()) {
    std::cerr << "No active session on port: " << device_id << std::endl;
    return OdResult::ERROR_UNKNOWN;
  }

//  if (!device_sessions[device]->SetFan("fan(1, 0)", 1)) {
//    std::cerr << "Failed to set FAN." << std::endl;
//    return OdResult::ERROR_UNKNOWN;
//  }

  if (!device_sessions[device]->StopThreadFunc()) {
    std::cerr << "Failed to stop tread" << std::endl;
    return OdResult::ERROR_UNKNOWN;
  }

  // Close the session and remove it from the map
  device_sessions[device]->Close();
  device_sessions.erase(device);

  return OdResult::SUCCESS;
}

OLFACTORY_DEVICE_API OdResult sony_odSetScentOrientation(const char* device_id, float yaw, float pitch) {
  return OdResult::ERROR_FUNCTION_UNSUPPORTED;
}

OLFACTORY_DEVICE_API OdResult sony_odStartScentEmission(const char* device_id, const char* scent_name, float level) {
  std::string device(device_id);

  // Check if a session is active for the given device_id
  if (device_sessions.find(device) == device_sessions.end() || !device_sessions[device]->IsConnected()) {
    std::cerr << "No active session on port: " << device_id << ". Start a session first.\n";
    return OdResult::ERROR_UNKNOWN;
  }

  // Send the command to start scent emission
  std::string s_scent(scent_name);
  int i_level = static_cast<int>(level * 10);
  std::string s_level = std::to_string(i_level);

  std::string command = "release(" + s_scent + ", " + s_level + ")";
  long long wait = static_cast<long long>(i_level + THREAD_SCENT_WAIT);
  if (!device_sessions[device]->SetScent(command, wait)) {
    std::cerr << "Failed to set SCENT." << std::endl;
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

  // Send the command to start scent emission
  std::string command = "";
  long long wait = THREAD_SCENT_WAIT;
  if (!device_sessions[device]->SetScent(command, wait)) {
    std::cerr << "Failed to set SCENT." << std::endl;
    return OdResult::ERROR_UNKNOWN;
  }

  return OdResult::SUCCESS;
}

}  // namespace sony::olfactory_device
