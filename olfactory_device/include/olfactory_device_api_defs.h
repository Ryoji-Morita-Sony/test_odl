#pragma once

#include <stdint.h>

#include <cmath>

/**
 * @def HELLO_WORLD_API
 * DLLライブラリのエクスポート/インポートキーワードを定義する。
 */
#ifndef HELLO_WORLD_API
#ifdef _WIN64
#ifdef BUILD_DLL
#define HELLO_WORLD_API extern "C" __declspec(dllexport)
#else
#define HELLO_WORLD_API extern "C" __declspec(dllimport)
#endif
#else
#define HELLO_WORLD_API
#endif
#endif

#pragma region ENUM_DEFINITION
/** APIの結果 */
enum class HwResult : int32_t {
  SUCCESS = 0,                     /**< 成功 */
  ERROR_LIBRARY_NOT_FOUND = -1,    /**< ライブラリがインストールされていない */
  ERROR_FUNCTION_UNSUPPORTED = -2, /**< 非サポートのAPI呼び出しによる失敗 */
  ERROR_UNKNOWN = -3,              /**< 不明なエラー */
  MAX = INT32_MAX
};
#pragma endregion ENUM_DEFINITION
