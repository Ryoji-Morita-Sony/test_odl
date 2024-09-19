// Copyright (c) 2024 Sony Corporation
//
// This source code is unlicensed and all use is prohibited without the express permission of the copyright
// holder.
//
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.

#pragma once

#include <windows.h>

namespace sony::olfactory_device {

class UARTInitializer {
 public:
  UARTInitializer();

  bool SendUARTCommand(const char* command);

 private:
  HANDLE hSerial;
};

}  // namespace sony::olfactory_device
