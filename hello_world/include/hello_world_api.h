#pragma once

#include "hello_world_api_defs.h"

namespace sony::training::hello_world {

/**
 * @brief メッセージ"Hello World"を返す
 * @param[in]  size msg_bufのサイズ
 * @param[out] msg_buf  メッセージの文字列を格納するバッファ。あらかじめメモリを確保すること
 * @return HelloWorldResult
 */
HELLO_WORLD_API HwResult sony_training_hwGetMessage(const uint32_t size, char* msg_buf);

}  // namespace sony::avatar_pose
