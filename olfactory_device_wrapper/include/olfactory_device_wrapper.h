// Copyright (c) 2024 Sony Corporation
//
// This source code is unlicensed and all use is prohibited without the express permission of the copyright
// holder.
//
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.

#pragma once

#include "olfactory_device_api_defs.h"

namespace sony::olfactory_device {

/**
 * @brief ライブラリをロードする。
 * @return OdResult
 */
OdResult LoadRuntimeLibrary();

/**
 * @brief ライブラリをアンロードする。
 */
void UnloadRuntimeLibrary();

/**
 * @brief メッセージ"Olfactory Device"を返す
 * @param[in]  size msg_bufのサイズ
 * @param[out] msg_buf  メッセージの文字列を格納するバッファ。あらかじめメモリを確保すること
 * @return OdResult
 */
OdResult GetMessage(const uint32_t size, char* msg_buf);

}  // namespace sony::olfactory_device
