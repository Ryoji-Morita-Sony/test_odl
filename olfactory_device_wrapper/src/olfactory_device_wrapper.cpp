// Copyright (c) 2024 Sony Corporation
//
// This source code is unlicensed and all use is prohibited without the express permission of the copyright
// holder.
//
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.

#include "olfactory_device_wrapper.h"

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

DLL_FUNC_DEFINE(sony_odStartScentEmission, int, const char*, int)
DLL_FUNC_DEFINE(sony_odStopScentEmission)

/** Get the installation path from a registry key */
std::wstring GetInstallPath() {
  HKEY hkey = HKEY_LOCAL_MACHINE;
  const std::wstring sub_key = std::wstring(L"SOFTWARE\\Sony Corporation\\OlfactoryDevice");
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
  GET_FUNCTION(sony_odStartScentEmission);
  GET_FUNCTION(sony_odStopScentEmission);
#pragma warning(pop)

#undef GET_FUNCTION

  return OdResult::SUCCESS;
}

void UnloadRuntimeLibrary() {
  ::FreeLibrary(handle);
  handle = nullptr;
}

OdResult StartScentEmission(int device_id, const char* scent_name, int intensity) {
  if (!IsRuntimeLibraryValid()) {
    return OdResult::ERROR_LIBRARY_NOT_FOUND;
  }

  if (sony_odStartScentEmission == nullptr) {
    return OdResult::ERROR_FUNCTION_UNSUPPORTED;
  }

  return sony_odStartScentEmission(device_id, scent_name, intensity);
}

OdResult StopScentEmission() {
  if (!IsRuntimeLibraryValid()) {
    return OdResult::ERROR_LIBRARY_NOT_FOUND;
  }

  if (sony_odStopScentEmission == nullptr) {
    return OdResult::ERROR_FUNCTION_UNSUPPORTED;
  }

  return sony_odStopScentEmission();
}

}  // namespace sony::olfactory_device
