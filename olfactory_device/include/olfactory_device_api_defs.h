// Copyright (c) 2024 Sony Corporation
//
// This source code is unlicensed and all use is prohibited without the express permission of the copyright
// holder.
//
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.

#pragma once

#include <stdint.h>

#include <cmath>

/**
 * @def OLFACTORY_DEVICE_API
 * DLLライブラリのエクスポート/インポートキーワードを定義する。
 */
#ifndef OLFACTORY_DEVICE_API
#ifdef _WIN64
#ifdef BUILD_DLL
#define OLFACTORY_DEVICE_API extern "C" __declspec(dllexport)
#else
#define OLFACTORY_DEVICE_API extern "C" __declspec(dllimport)
#endif
#else
#define OLFACTORY_DEVICE_API
#endif
#endif

#pragma region ENUM_DEFINITION
/** APIの結果 */
enum class OdResult : int32_t {
  SUCCESS = 0,                     /**< 成功 */
  ERROR_LIBRARY_NOT_FOUND = -1,    /**< ライブラリがインストールされていない */
  ERROR_FUNCTION_UNSUPPORTED = -2, /**< 非サポートのAPI呼び出しによる失敗 */
  ERROR_UNKNOWN = -3,              /**< 不明なエラー */
  MAX = INT32_MAX
};
#pragma endregion ENUM_DEFINITION
