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
#include "picojson.h"
#include "uart_session.h"
#include "stub_session.h"
#include "osc_session.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <iomanip> // for std::setw, std::setfill
#include <string>
#include <functional>

// Third Party Libraries
#include <spdlog/spdlog.h>
#include <log-settings/log_settings.h>

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

// Uncomment to be enabled Thread
//#define ENABLED_THREAD


#define FILE_DEVICE_JSON ("C:\\VisualStudioProjects\\device.json")

// Map to manage DeviceSessionIF instances by device_id
static std::unordered_map<std::string, std::unique_ptr<DeviceSessionIF>> device_sessions;

// Struct to store emission and cooldown end times
struct DeviceScent {
  std::chrono::steady_clock::time_point emission_end_time;  // End of emission
  std::chrono::steady_clock::time_point cooldown_end_time;  // End of cooldown
  float duration;                                           // Duration for the current emission
};

struct DeviceTimes {
  DeviceScent scent0;
  DeviceScent scent1;
  DeviceScent scent2;
  DeviceScent scent3;
};

// Map to track the emission and cooldown times for each device
std::unordered_map<std::string, DeviceTimes> device_next_available_time;


// Static wrapper function to call the user-defined log callback
static void LogCallbackWrapper(const char* message, SonyOzLogSettings_LogLevels level,
                               OdLogCallback userCallback) {
  if (userCallback) {
    userCallback(message, static_cast<OdLogLevel>(static_cast<int32_t>(level)));
  }
}

// Global variable to store the user-defined callback
static OdLogCallback g_userLogCallback = nullptr;

OLFACTORY_DEVICE_API OdResult sony_odRegisterLogCallback(OdLogCallback callback) {
  SonyOzLogSettings_Logger logger;

  // Store the user callback in the global variable
  g_userLogCallback = callback;

  // Pass the wrapper lambda to InitializeCallbackLoggerSettings
  sony::oz::log_settings::InitializeCallbackLoggerSettings(
      [](const char* message, SonyOzLogSettings_LogLevels level) {
        LogCallbackWrapper(message, level, g_userLogCallback);
      },
      &logger);

  // Enable all log levels because caller will filter the log levels by itself.
  spdlog::set_level(spdlog::level::trace);

  return OdResult::SUCCESS;
}

static OdResult CtrlDevice(std::string device, std::vector<std::string> vec) {
  for (const auto& cmd : vec) {
    if (!device_sessions[device]->SendData(cmd)) {
      std::cerr << "Failed to send a command." << std::endl;
      return OdResult::ERROR_UNKNOWN;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
  return OdResult::SUCCESS;
}

OLFACTORY_DEVICE_API OdResult sony_odStartSession(const char* device_id) {
  spdlog::debug("{} called.", __func__);
  std::string device(device_id);

  // Check if a session is already active for the given device_id
  if (device_sessions.find(device) != device_sessions.end() && device_sessions[device]->IsConnected()) {
    spdlog::error("Session is already active on port: {}", device_id);
    return OdResult::ERROR_UNKNOWN;
  }

  // Emplace the new SessionType (either StubSession or UartSession)
  device_sessions.emplace(device, std::make_unique<SessionType>());
  DeviceTimes time;
  device_next_available_time.emplace(device, time);

  // Open the session for the newly created session instance
  if (!device_sessions[device]->Open(device_id)) {
    spdlog::error("Failed to open connection on port: {}", device_id);
    device_sessions.erase(device);  // Remove if failed
    return OdResult::ERROR_UNKNOWN;
  }

  std::vector<std::string> vec = {"motor(0, 30)", "motor(1, 30)"};
  CtrlDevice(device, vec);

  spdlog::debug("{} completed.", __func__);
  return OdResult::SUCCESS;
}

OLFACTORY_DEVICE_API OdResult sony_odEndSession(const char* device_id) {
  spdlog::debug("{} called.", __func__);
  std::string device(device_id);

  // Check if a session is active for the given device_id
  if (device_sessions.find(device) == device_sessions.end() || !device_sessions[device]->IsConnected()) {
    spdlog::error("No active session on port: {}", device_id);
    return OdResult::ERROR_UNKNOWN;
  }

//  std::vector<std::string> vec = {"motor(0, 0)", "motor(1, 0)", "reset(0, 0)"};
  std::vector<std::string> vec = {"motor(0, 0)", "motor(1, 0)"};
  CtrlDevice(device, vec);

  // Close the session and remove it from the map
  device_sessions[device]->Close();
  device_sessions.erase(device);

  spdlog::debug("{} completed.", __func__);
  return OdResult::SUCCESS;
}

OLFACTORY_DEVICE_API OdResult sony_odSetScentOrientation(const char* device_id, float yaw, float pitch) {
  spdlog::debug("{} called.", __func__);
  spdlog::debug("{} completed.", __func__);
  return OdResult::ERROR_FUNCTION_UNSUPPORTED;
}

OLFACTORY_DEVICE_API OdResult sony_odStartScentEmission(const char* device_id, const char* scent_name, float duration, bool& is_available) {
  spdlog::debug("{} called.", __func__);
  std::string device(device_id);
  std::string scent(scent_name);
  int i_device = std::stoi(device);
  int i_scent = std::stoi(scent);

  // Check if a session is active for the given device_id
  if (device_sessions.find(device) == device_sessions.end() || !device_sessions[device]->IsConnected()) {
    spdlog::error("No active session on port: {}. Start a session first.", device_id);
    return OdResult::ERROR_UNKNOWN;
  }

  // Clamp the duration to the range [0, 10]
  duration = std::clamp(duration, 0.0f, 10.0f);
  // Get the current time
  auto now = std::chrono::steady_clock::now();
  // Check the last start time for the given device
  auto it = device_next_available_time.find(device);
  if (it != device_next_available_time.end()) {
    if (i_scent == 0) {
      if (now < it->second.scent0.cooldown_end_time) {
        // Device is still unavailable
        is_available = false;
        spdlog::debug("{} Device is still unavailable.", __func__);
        return OdResult::SUCCESS;
      } else {
        is_available = true;
        std::string command = "release(0," + std::to_string(static_cast<int>(duration)) + ")";
        if (!device_sessions[device]->SendData(command)) {
          spdlog::error("Failed to set SCENT.");
          return OdResult::ERROR_UNKNOWN;
        }
        // Calculate emission_end_time and cooldown_end_time
        auto emission_end_time = now               + std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<float>(duration));
        auto cooldown_end_time = emission_end_time + std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<float>(6.0f));
        // Update the times and duration for the device
        device_next_available_time[device].scent0 = {emission_end_time, cooldown_end_time, duration};
      }
    } else if (i_scent == 1) {
      if (now < it->second.scent1.cooldown_end_time) {
        // Device is still unavailable
        is_available = false;
        spdlog::debug("{} Device is still unavailable.", __func__);
        return OdResult::SUCCESS;
      } else {
        is_available = true;
        std::string command = "release(1," + std::to_string(static_cast<int>(duration)) + ")";
        if (!device_sessions[device]->SendData(command)) {
          spdlog::error("Failed to set SCENT.");
          return OdResult::ERROR_UNKNOWN;
        }
        // Calculate emission_end_time and cooldown_end_time
        auto emission_end_time = now               + std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<float>(duration));
        auto cooldown_end_time = emission_end_time + std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<float>(6.0f));
        // Update the times and duration for the device
        device_next_available_time[device].scent1 = {emission_end_time, cooldown_end_time, duration};
      }
    } else if (i_scent == 2) {
      if (now < it->second.scent2.cooldown_end_time) {
        // Device is still unavailable
        is_available = false;
        spdlog::debug("{} Device is still unavailable.", __func__);
        return OdResult::SUCCESS;
      } else {
        is_available = true;
        std::string command = "release(2," + std::to_string(static_cast<int>(duration)) + ")";
        if (!device_sessions[device]->SendData(command)) {
          spdlog::error("Failed to set SCENT.");
          return OdResult::ERROR_UNKNOWN;
        }
        // Calculate emission_end_time and cooldown_end_time
        auto emission_end_time = now               + std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<float>(duration));
        auto cooldown_end_time = emission_end_time + std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<float>(6.0f));
        // Update the times and duration for the device
        device_next_available_time[device].scent2 = {emission_end_time, cooldown_end_time, duration};
      }
    } else if (i_scent == 3) {
      if (now < it->second.scent3.cooldown_end_time) {
        // Device is still unavailable
        is_available = false;
        spdlog::debug("{} Device is still unavailable.", __func__);
        return OdResult::SUCCESS;
      } else {
        is_available = true;
        std::string command = "release(3," + std::to_string(static_cast<int>(duration)) + ")";
        if (!device_sessions[device]->SendData(command)) {
          spdlog::error("Failed to set SCENT.");
          return OdResult::ERROR_UNKNOWN;
        }
        // Calculate emission_end_time and cooldown_end_time
        auto emission_end_time = now               + std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<float>(duration));
        auto cooldown_end_time = emission_end_time + std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<float>(6.0f));
        // Update the times and duration for the device
        device_next_available_time[device].scent3 = {emission_end_time, cooldown_end_time, duration};
      }
    }
  }
  spdlog::debug("{} completed.", __func__);
  return OdResult::SUCCESS;
}

OLFACTORY_DEVICE_API OdResult sony_odStopScentEmission(const char* device_id) {
  spdlog::debug("{} called.", __func__);
  std::string device(device_id);

  // Check if a session is active for the given device_id
  if (device_sessions.find(device) == device_sessions.end() || !device_sessions[device]->IsConnected()) {
    spdlog::error("No active session on port: {}. Start a session first.", device_id);
    return OdResult::ERROR_UNKNOWN;
  }

  std::vector<std::string> vec = {"release(0, 0)", "release(1, 0)", "release(2, 0)", "release(3, 0)"};
  CtrlDevice(device, vec);

  spdlog::debug("{} completed.", __func__);
  return OdResult::SUCCESS;
}

OLFACTORY_DEVICE_API OdResult sony_odIsScentEmissionAvailable(const char* device_id, bool& is_available) {
  spdlog::debug("{} called.", __func__);
  std::string device(device_id);

  // Check if a session is active for the given device_id
  if (device_sessions.find(device) == device_sessions.end() || !device_sessions[device]->IsConnected()) {
    spdlog::error("No active session on port: {}. Start a session first.", device_id);
    return OdResult::ERROR_UNKNOWN;
  }

  // Check if scent emission is available
  is_available = device_sessions[device]->IsScentEmissionAvailable();

  spdlog::debug("{} completed.", __func__);
  return OdResult::SUCCESS;
}

}  // namespace sony::olfactory_device
