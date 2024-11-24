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

// Uncomment to use the registry key's path
//#define USE_JSON_PATH_FROM_REGISTRY_KEY


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

#ifdef USE_JSON_PATH_FROM_REGISTRY_KEY
/** Get the installation path from a registry key */
std::wstring GetInstallPath() {
  HKEY hkey = HKEY_LOCAL_MACHINE;
  const std::wstring sub_key = std::wstring(L"SOFTWARE\\Sony Corporation\\Olfactory");
  const std::wstring value = L"Path";

  DWORD data_size{};
  LONG return_code =
      ::RegGetValueW(hkey, sub_key.c_str(), value.c_str(), RRF_RT_REG_SZ, nullptr, nullptr, &data_size);
  if (return_code != ERROR_SUCCESS) {
    return L"";
  }

  std::wstring data;
  data.resize(data_size / sizeof(wchar_t));

  return_code =
      ::RegGetValueW(hkey, sub_key.c_str(), value.c_str(), RRF_RT_REG_SZ, nullptr, &data[0], &data_size);
  if (return_code != ERROR_SUCCESS) {
    return L"";
  }

  DWORD string_length_in_wchars = data_size / sizeof(wchar_t);

  // Exclude the NULL written by the Win32 API
  string_length_in_wchars--;

  data.resize(string_length_in_wchars);
  return data;
}
#endif

std::tuple<std::string, int, int, int> ParseJson(std::string id) {

// JSON file
#ifdef USE_JSON_PATH_FROM_REGISTRY_KEY
  std::wstring directry = GetInstallPath();
  std::wstring path = directry + L"json\\device.json";
  std::ifstream inputFile(path);
#else
  std::ifstream inputFile(FILE_DEVICE_JSON);
#endif

  if (!inputFile) {
    std::cerr << "Failed to open device.json." << std::endl;
    return std::make_tuple("file NG", 0, 0, 0);
  }
  std::stringstream buffer;
  buffer << inputFile.rdbuf();
  std::string json = buffer.str();
  // std::cout << json << std::endl;

  // parse JSON
  picojson::value v;
  std::string err = picojson::parse(v, json);
  if (!err.empty()) {
    std::cerr << "JSON parse error: " << err << std::endl;
    return std::make_tuple("file NG", 0, 0, 0);
  }

  std::string ip;
  int scent0;
  int scent1;
  int motor;

  // Get device info
  const picojson::array& devices = v.get("device").get<picojson::array>();
  for (const auto& device : devices) {
    if (id == device.get("id").get<std::string>()) {
      ip = device.get("ip").get<std::string>();
      scent0 = static_cast<int>(device.get("scent0").get<double>());
      scent1 = static_cast<int>(device.get("scent1").get<double>());
      motor  = static_cast<int>(device.get("motor").get<double>());
    }
  }

  return std::make_tuple(ip, scent0, scent1, motor);
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
  std::string id(device_id);
  std::string ip = "";
  int scent0 = 0;
  int scent1 = 0;
  int motor = 0;

  std::tie(ip, scent0, scent1, motor) = ParseJson(id);
  spdlog::debug("{}({}): {} called.", id, ip, __func__);

  // Check if a session is already active for the given device_id
  if (device_sessions.find(ip) != device_sessions.end() && device_sessions[ip]->IsConnected()) {
    spdlog::debug("{}({}): Session is already active on port", id, ip);
    return OdResult::SUCCESS;
  }

  // Emplace the new SessionType (either StubSession or UartSession)
  device_sessions.emplace(ip, std::make_unique<SessionType>());
  DeviceTimes time;
  device_next_available_time.emplace(ip, time);

  // Open the session for the newly created session instance
  if (!device_sessions[ip]->Open(ip.c_str())) {
    spdlog::error("{}({}): Failed to open connection on port", id, ip);
    device_sessions.erase(ip);  // Remove if failed
    return OdResult::ERROR_UNKNOWN;
  }

  std::vector<std::string> vec = {"motor(0, 30)", "motor(1, 30)"};
  CtrlDevice(ip, vec);

  spdlog::debug("{}({}): {} completed.", id, ip, __func__);
  return OdResult::SUCCESS;
}

OLFACTORY_DEVICE_API OdResult sony_odEndSession(const char* device_id) {
  std::string id(device_id);
  std::string ip = "";
  int scent0 = 0;
  int scent1 = 0;
  int motor = 0;

  std::tie(ip, scent0, scent1, motor) = ParseJson(id);
  spdlog::debug("{}({}): {} called.", id, ip, __func__);

  // Check if a session is active for the given device_id
  if (device_sessions.find(ip) == device_sessions.end() || !device_sessions[ip]->IsConnected()) {
    spdlog::error("{}({}): No active session on port", id, ip);
    return OdResult::SUCCESS;
  }

//  std::vector<std::string> vec = {"motor(0, 0)", "motor(1, 0)", "reset(0, 0)"};
  std::vector<std::string> vec = {"motor(0, 0)", "motor(1, 0)"};
  CtrlDevice(ip, vec);

  // Close the session and remove it from the map
  device_sessions[ip]->Close();
  device_sessions.erase(ip);

  spdlog::debug("{}({}): {} completed.", id, ip, __func__);
  return OdResult::SUCCESS;
}

OLFACTORY_DEVICE_API OdResult sony_odSetScentOrientation(const char* device_id, float yaw, float pitch) {
  std::string id(device_id);
  std::string ip = "";
  int scent0 = 0;
  int scent1 = 0;
  int motor = 0;

  std::tie(ip, scent0, scent1, motor) = ParseJson(id);
  spdlog::debug("{}({}): {} called.", id, ip, __func__);
  spdlog::debug("{}({}): {} completed.", id, ip, __func__);
  return OdResult::ERROR_FUNCTION_UNSUPPORTED;
}

OLFACTORY_DEVICE_API OdResult sony_odStartScentEmission(const char* device_id, const char* scent_name, float duration, bool& is_available) {
  std::string id(device_id);
  std::string ip = "";
  int scent0 = 0;
  int scent1 = 0;
  int motor = 0;

  std::tie(ip, scent0, scent1, motor) = ParseJson(id);
  spdlog::debug("{}({}): {} called.", id, ip, __func__);

  std::string scent(scent_name);
  int i_scent = std::stoi(scent);

  // Check if a session is active for the given device_id
  if (device_sessions.find(ip) == device_sessions.end() || !device_sessions[ip]->IsConnected()) {
    spdlog::error("{}({}): No active session on port. Start a session first.", id, ip);
    return OdResult::ERROR_UNKNOWN;
  }

  // Clamp the duration to the range [0, 10]
  duration = std::clamp(duration, 0.0f, 10.0f);
  // Get the current time
  auto now = std::chrono::steady_clock::now();
  // Check the last start time for the given device
  auto it = device_next_available_time.find(ip);
  if (it != device_next_available_time.end()) {
    if (i_scent == 0 && scent0 == 0) {
      now = std::chrono::steady_clock::now();
      if (now < it->second.scent0.cooldown_end_time) {
        // Device is still unavailable
        is_available = false;
        spdlog::debug("{}({}): {} Device is still unavailable.", id, ip, __func__);
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        std::cout << "[OscSession] Data sent: " << id << "(" << ip << ")" << "Device is still unavailable." << std::endl;
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return OdResult::SUCCESS;
      } else {
        is_available = false;
        std::string command = "release(0," + std::to_string(static_cast<int>(duration)) + ")";
        if (!device_sessions[ip]->SendData(command)) {
          spdlog::error("{}({}): Failed to set SCENT.", id, ip);
          return OdResult::ERROR_UNKNOWN;
        }
        // Calculate emission_end_time and cooldown_end_time
        now = std::chrono::steady_clock::now();
        auto emission_end_time = now               + std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<float>(duration));
        auto cooldown_end_time = emission_end_time + std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<float>(6.0f));
        // Update the times and duration for the device
        device_next_available_time[ip].scent0 = {emission_end_time, cooldown_end_time, duration};
      }
    }
    
    if (i_scent == 1 && scent1 == 1) {
      now = std::chrono::steady_clock::now();
      if (now < it->second.scent1.cooldown_end_time) {
        // Device is still unavailable
        is_available = false;
        spdlog::debug("{}({}): {} Device is still unavailable.", id, ip, __func__);
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        std::cout << "[OscSession] Data sent: " << id << "(" << ip << ")" << "Device is still unavailable." << std::endl;
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return OdResult::SUCCESS;
      } else {
        is_available = false;
        std::string command = "release(1," + std::to_string(static_cast<int>(duration)) + ")";
        if (!device_sessions[ip]->SendData(command)) {
          spdlog::error("{}({}): Failed to set SCENT.", id, ip);
          return OdResult::ERROR_UNKNOWN;
        }
        // Calculate emission_end_time and cooldown_end_time
        now = std::chrono::steady_clock::now();
        auto emission_end_time = now               + std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<float>(duration));
        auto cooldown_end_time = emission_end_time + std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<float>(6.0f));
        // Update the times and duration for the device
        device_next_available_time[ip].scent1 = {emission_end_time, cooldown_end_time, duration};
      }
    }

    if (i_scent == 0 && scent0 == 2) {
      now = std::chrono::steady_clock::now();
      if (now < it->second.scent2.cooldown_end_time) {
        // Device is still unavailable
        is_available = false;
        spdlog::debug("{}({}): {} Device is still unavailable.", id, ip, __func__);
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        std::cout << "[OscSession] Data sent: " << id << "(" << ip << ")" << "Device is still unavailable." << std::endl;
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return OdResult::SUCCESS;
      } else {
        is_available = false;
        std::string command = "release(2," + std::to_string(static_cast<int>(duration)) + ")";
        if (!device_sessions[ip]->SendData(command)) {
          spdlog::error("{}({}): Failed to set SCENT.", id, ip);
          return OdResult::ERROR_UNKNOWN;
        }
        // Calculate emission_end_time and cooldown_end_time
        now = std::chrono::steady_clock::now();
        auto emission_end_time = now               + std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<float>(duration));
        auto cooldown_end_time = emission_end_time + std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<float>(6.0f));
        // Update the times and duration for the device
        device_next_available_time[ip].scent2 = {emission_end_time, cooldown_end_time, duration};
      }
    }

    if (i_scent == 1 && scent1 == 3) {
      now = std::chrono::steady_clock::now();
      if (now < it->second.scent3.cooldown_end_time) {
        // Device is still unavailable
        is_available = false;
        spdlog::debug("{}({}): {} Device is still unavailable.", id, ip, __func__);
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        std::cout << "[OscSession] Data sent: " << id << "(" << ip << ")" << "Device is still unavailable." << std::endl;
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return OdResult::SUCCESS;
      } else {
        is_available = false;
        std::string command = "release(3," + std::to_string(static_cast<int>(duration)) + ")";
        if (!device_sessions[ip]->SendData(command)) {
          spdlog::error("{}({}): Failed to set SCENT.", id, ip);
          return OdResult::ERROR_UNKNOWN;
        }
        // Calculate emission_end_time and cooldown_end_time
        now = std::chrono::steady_clock::now();
        auto emission_end_time = now               + std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<float>(duration));
        auto cooldown_end_time = emission_end_time + std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<float>(6.0f));
        // Update the times and duration for the device
        device_next_available_time[ip].scent3 = {emission_end_time, cooldown_end_time, duration};
      }
    }
  }
  spdlog::debug("{}({}): {} completed.", id, ip, __func__);
  return OdResult::SUCCESS;
}

OLFACTORY_DEVICE_API OdResult sony_odStopScentEmission(const char* device_id) {
  std::string id(device_id);
  std::string ip = "";
  int scent0 = 0;
  int scent1 = 0;
  int motor = 0;

  std::tie(ip, scent0, scent1, motor) = ParseJson(id);
  spdlog::debug("{}({}): {} called.", id, ip, __func__);

  // Check if a session is active for the given device_id
  if (device_sessions.find(ip) == device_sessions.end() || !device_sessions[ip]->IsConnected()) {
    spdlog::error("{}({}): No active session on port. Start a session first.", id, ip);
    return OdResult::ERROR_UNKNOWN;
  }

  std::string command;
  command = "release(" + std::to_string(scent0) + ", 0)";
  if (!device_sessions[ip]->SendData(command)) {
    spdlog::error("{}({}): Failed to set SCENT.", id, ip);
    return OdResult::ERROR_UNKNOWN;
  }

  command = "release(" + std::to_string(scent1) + ", 0)";
  if (!device_sessions[ip]->SendData(command)) {
    spdlog::error("{}({}): Failed to set SCENT.", id, ip);
    return OdResult::ERROR_UNKNOWN;
  }

  // Check if the device has an active start time
  auto it = device_next_available_time.find(ip);
  if (it != device_next_available_time.end()) {
    device_next_available_time.erase(it);
  }

  spdlog::debug("{}({}): {} completed.", id, ip, __func__);
  return OdResult::SUCCESS;
}

OLFACTORY_DEVICE_API OdResult sony_odIsScentEmissionAvailable(const char* device_id, bool& is_available) {
  std::string id(device_id);
  std::string ip = "";
  int scent0 = 0;
  int scent1 = 0;
  int motor = 0;

  std::tie(ip, scent0, scent1, motor) = ParseJson(id);
//  spdlog::debug("{}({}): {} called.", id, ip, __func__);

  // Check if a session is active for the given device_id
  if (device_sessions.find(ip) == device_sessions.end() || !device_sessions[ip]->IsConnected()) {
    spdlog::error("{}({}): {} : No active session on port. Start a session first.", id, ip, __func__);
    return OdResult::ERROR_UNKNOWN;
  }

  // Get the current time
  auto now = std::chrono::steady_clock::now();

  // Check the last start time for the given device
  auto it = device_next_available_time.find(ip);
  if (it != device_next_available_time.end()) {

    bool flag_scent0 = true;
    bool flag_scent1 = true;
    bool flag_scent2 = true;
    bool flag_scent3 = true;

    if (scent0 == 0) {
      if (now < it->second.scent0.cooldown_end_time) {
        flag_scent0 = false;
      } else {
        flag_scent0 = true;
      }
    }

    if (scent1 == 1) {
      if (now < it->second.scent1.cooldown_end_time) {
        flag_scent1 = false;
      } else {
        flag_scent1 = true;
      }
    }

    if (scent0 == 2) {
      if (now < it->second.scent2.cooldown_end_time) {
        flag_scent2 = false;
      } else {
        flag_scent2 = true;
      }
    }

    if (scent1 == 3) {
      if (now < it->second.scent3.cooldown_end_time) {
        flag_scent3 = false;
      } else {
        flag_scent3 = true;
      }
    }

    if (flag_scent0 == true && flag_scent1 == true && flag_scent2 == true && flag_scent3 == true) {
      is_available = true;
    } else {
      is_available = false;
    }

  }

  // Check if scent emission is available
//  is_available = device_sessions[ip]->IsScentEmissionAvailable();

//  spdlog::debug("{}({}): {} completed.", id, ip, __func__);
  return OdResult::SUCCESS;
}

}  // namespace sony::olfactory_device
