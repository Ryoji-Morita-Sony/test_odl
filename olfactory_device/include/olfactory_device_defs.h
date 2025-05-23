﻿/**
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

#include <stdint.h>

#include <cmath>

#pragma region ENUM_DEFINITION
/** APIの結果 */
enum class OdResult : int32_t {
  SUCCESS = 0,                     /**< 成功 */
  ERROR_LIBRARY_NOT_FOUND = -1,    /**< ライブラリがインストールされていない */
  ERROR_FUNCTION_UNSUPPORTED = -2, /**< 非サポートのAPI呼び出しによる失敗 */
  ERROR_UNKNOWN = -3,              /**< 不明なエラー */

  ERROR_SEND_DEVICE_COMMAND_FAILED,

  MAX = INT32_MAX
};

enum class OdLogLevel : int32_t {
  TRACE = 0,     ///< Trace
  DEBUG = 1,     ///< Debug
  INFO = 2,      ///< Info
  WARN = 3,      ///< Warn
  ERR = 4,       ///< Error
  CRITICAL = 5,  ///< Critical
  OFF = 6        ///< Off
};
#pragma endregion ENUM_DEFINITION

/**
 * @brief Log callback function type with log level
 * @param[in] level The log level of the message
 * @param[in] message The log message
 */
using OdLogCallback = void (*)(const char*, OdLogLevel);