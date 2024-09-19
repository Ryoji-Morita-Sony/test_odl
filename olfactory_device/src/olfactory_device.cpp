// Copyright (c) 2024 Sony Corporation
//
// This source code is unlicensed and all use is prohibited without the express permission of the copyright
// holder.
//
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.

#include "olfactory_device.h"

#include <stdio.h>

#include "uart_initializer.h"

namespace sony::olfactory_device {

// 静的オブジェクトでUARTを初期化
static UARTInitializer uart_initializer;  // ライブラリロード時にUARTを初期化

OLFACTORY_DEVICE_API OdResult sony_odStartScentEmission(int device_id, const char* scent_name,
                                                        int intensity) {
  // デバッグログ。実装が進んだら消してください。
  printf("Scent emission started for device %d with scent \"%s\" and intensity: %d.\n", device_id, scent_name,
         intensity);

  // UARTコマンドを準備
  char command[256];
  sprintf(command, "START %d %s %d\n", device_id, scent_name, intensity);

  // UARTコマンドを送信
  if (!uart_initializer.SendUARTCommand(command)) {
    return OdResult::ERROR_SEND_DEVICE_COMMAND_FAILED;
  }

  return OdResult::SUCCESS;
}

OLFACTORY_DEVICE_API OdResult sony_odStopScentEmission() {
  // デバッグログ。実装が進んだら消してください。
  printf("Scent emission stopped.\n");

  // UARTコマンドを準備
  char command[256];
  sprintf(command, "STOP\n");

  // UARTコマンドを送信
  if (!uart_initializer.SendUARTCommand(command)) {
    return OdResult::ERROR_SEND_DEVICE_COMMAND_FAILED;
  }

  return OdResult::SUCCESS;
}

}  // namespace sony::olfactory_device
