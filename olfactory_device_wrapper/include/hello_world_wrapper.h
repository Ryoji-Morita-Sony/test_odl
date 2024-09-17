#pragma once

#include "hello_world_api_defs.h"

namespace sony::training::hello_world {

/**
 * @brief ライブラリをロードする。
 * @return HwResult
 */
HwResult LoadRuntimeLibrary();

/**
 * @brief ライブラリをアンロードする。
 */
void UnloadRuntimeLibrary();

/**
 * @brief メッセージ"Hello World"を返す
 * @param[in]  size msg_bufのサイズ
 * @param[out] msg_buf  メッセージの文字列を格納するバッファ。あらかじめメモリを確保すること
 * @return HelloWorldResult
 */
HwResult GetMessage(const uint32_t size, char* msg_buf);

}
