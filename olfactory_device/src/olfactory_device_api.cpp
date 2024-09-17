// Copyright (c) 2024 Sony Corporation
//
// This source code is unlicensed and all use is prohibited without the express permission of the copyright
// holder.
//
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.

#include "olfactory_device_api.h"

#include <string.h>

namespace sony::olfactory_device {

OLFACTORY_DEVICE_API OdResult sony_odGetMessage(const uint32_t size, char* msg_buf) {
  strcpy_s(msg_buf, size, "Olfactory Device");
  return OdResult::SUCCESS;
}

}  // namespace sony::olfactory_device
