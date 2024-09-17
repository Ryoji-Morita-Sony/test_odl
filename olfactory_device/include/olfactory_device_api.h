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
 * @brief 匂いの放出を開始する
 * @param[in] device_id デバイスのID
 * @param[in] scent_name 匂いの名称
 * @param[in] intensity 匂いの強度（例: 0〜100）
 * @return OdResult
 */
OLFACTORY_DEVICE_API OdResult StartScentEmission(int device_id, const char* scent_name, int intensity);


/**
 * @brief 芳香の放出を停止する
 * @return OdResult
 */
OLFACTORY_DEVICE_API OdResult StopScentEmission();

}  // namespace sony::olfactory_device
