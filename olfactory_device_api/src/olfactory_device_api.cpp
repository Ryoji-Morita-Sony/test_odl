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

#include "olfactory_device_api.h"

#include <string>

#pragma region UNREAL_ENGINE_WORKAROUND
// clang-format off
#ifdef UNREAL_ENGINE_PLUGIN
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/MinWindows.h"
#include "Windows/HideWindowsPlatformTypes.h"
#else
#include <Windows.h>
#endif
// clang-format on
#pragma endregion UNREAL_ENGINE_WORKAROUND

namespace {

/** DLL handle */
HMODULE handle = nullptr;

/** Check validity of handle */
bool IsRuntimeLibraryValid() { return handle != nullptr; }

/** Defines the type of function to get */
#define DLL_FUNC_DEFINE(name, ...)            \
  using name##_t = OdResult (*)(__VA_ARGS__); \
  name##_t name = nullptr;

DLL_FUNC_DEFINE(sony_odRegisterLogCallback, OdLogCallback)
DLL_FUNC_DEFINE(sony_odStartSession, const char*)
DLL_FUNC_DEFINE(sony_odEndSession, const char*)
DLL_FUNC_DEFINE(sony_odSetScentOrientation, const char*, float, float)
DLL_FUNC_DEFINE(sony_odStartScentEmission, const char*, const char*, float, bool&)
DLL_FUNC_DEFINE(sony_odStopScentEmission, const char*)
DLL_FUNC_DEFINE(sony_odIsScentEmissionAvailable, const char*, bool&)

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

}  // namespace

namespace sony::olfactory_device {

OdResult LoadRuntimeLibrary() {
  if (handle) {
    return OdResult::SUCCESS;
  }

  std::wstring directry = GetInstallPath();
  std::wstring path = directry + L"lib\\olfactory_device.dll";
  handle = ::LoadLibraryW(path.c_str());

  if (handle == nullptr) {
    std::wstring message = L"Failed to load olfactory_device.dll.\n Please check " + path;
    ::MessageBoxW(NULL, message.c_str(), L"Error", 0);
    return OdResult::ERROR_LIBRARY_NOT_FOUND;
  }

#define GET_FUNCTION(name)                        \
  name = (name##_t)GetProcAddress(handle, #name); \
  if (!name) {                                    \
    return OdResult::ERROR_FUNCTION_UNSUPPORTED;  \
  }

#pragma warning(push)
#pragma warning(disable : 4191)
  GET_FUNCTION(sony_odRegisterLogCallback);
  GET_FUNCTION(sony_odStartSession);
  GET_FUNCTION(sony_odEndSession);
  GET_FUNCTION(sony_odSetScentOrientation);
  GET_FUNCTION(sony_odStartScentEmission);
  GET_FUNCTION(sony_odStopScentEmission);
  GET_FUNCTION(sony_odIsScentEmissionAvailable);
#pragma warning(pop)

#undef GET_FUNCTION

  return OdResult::SUCCESS;
}

void UnloadRuntimeLibrary() {
  ::FreeLibrary(handle);
  handle = nullptr;
}

OdResult RegisterLogCallback(OdLogCallback callback) {
  if (!IsRuntimeLibraryValid()) {
    return OdResult::ERROR_LIBRARY_NOT_FOUND;
  }

  if (sony_odRegisterLogCallback == nullptr) {
    return OdResult::ERROR_FUNCTION_UNSUPPORTED;
  }

  return sony_odRegisterLogCallback(callback);
}

OdResult StartSession(const char* device_id) {
  if (!IsRuntimeLibraryValid()) {
    return OdResult::ERROR_LIBRARY_NOT_FOUND;
  }

  if (sony_odStartSession == nullptr) {
    return OdResult::ERROR_FUNCTION_UNSUPPORTED;
  }

  return sony_odStartSession(device_id);
}

OdResult EndSession(const char* device_id) {
  if (!IsRuntimeLibraryValid()) {
    return OdResult::ERROR_LIBRARY_NOT_FOUND;
  }

  if (sony_odEndSession == nullptr) {
    return OdResult::ERROR_FUNCTION_UNSUPPORTED;
  }

  return sony_odEndSession(device_id);
}

OdResult StartScentEmission(const char* device_id, const char* scent_name, float duration, bool& is_available) {
  if (!IsRuntimeLibraryValid()) {
    return OdResult::ERROR_LIBRARY_NOT_FOUND;
  }

  if (sony_odStartScentEmission == nullptr) {
    return OdResult::ERROR_FUNCTION_UNSUPPORTED;
  }

  return sony_odStartScentEmission(device_id, scent_name, duration, is_available);
}

OdResult StopScentEmission(const char* device_id) {
  if (!IsRuntimeLibraryValid()) {
    return OdResult::ERROR_LIBRARY_NOT_FOUND;
  }

  if (sony_odStopScentEmission == nullptr) {
    return OdResult::ERROR_FUNCTION_UNSUPPORTED;
  }

  return sony_odStopScentEmission(device_id);
}

OdResult IsScentEmissionAvailable(const char* device_id, bool& is_avaiable) {
  if (!IsRuntimeLibraryValid()) {
    return OdResult::ERROR_LIBRARY_NOT_FOUND;
  }

  if (sony_odIsScentEmissionAvailable == nullptr) {
    return OdResult::ERROR_FUNCTION_UNSUPPORTED;
  }

  return sony_odIsScentEmissionAvailable(device_id, is_avaiable);
}

OdResult SetScentOrientation(const char* device_id, float yaw, float pitch) {
  if (!IsRuntimeLibraryValid()) {
    return OdResult::ERROR_LIBRARY_NOT_FOUND;
  }

  if (sony_odSetScentOrientation == nullptr) {
    return OdResult::ERROR_FUNCTION_UNSUPPORTED;
  }

  return sony_odSetScentOrientation(device_id, yaw, pitch);
}

}  // namespace sony::olfactory_device
