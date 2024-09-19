// Copyright (c) 2024 Sony Corporation
//
// This source code is unlicensed and all use is prohibited without the express permission of the copyright
// holder.
//
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.

#include "olfactory_device_api.h"

#include <stdio.h>

namespace sony::olfactory_device {

OLFACTORY_DEVICE_API OdResult sony_odStartScentEmission(int device_id, const char* scent_name,
                                                        int intensity) {
  // ÀÛ‚Ì“õ‚¢‚Ì•úoˆ—‚ğÀ‘•
  printf("Scent emission started for device %d with scent \"%s\" and intensity: %d.\n", device_id, scent_name,
         intensity);
  return OdResult::SUCCESS;
}

OLFACTORY_DEVICE_API OdResult sony_odStopScentEmission() {
  // ÀÛ‚Ì–F‚Ì•úo’â~ˆ—‚ğÀ‘•
  printf("Scent emission stopped.\n");
  return OdResult::SUCCESS;
}

}  // namespace sony::olfactory_device
