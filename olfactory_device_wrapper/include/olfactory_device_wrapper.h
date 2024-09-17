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
 * @brief ���C�u���������[�h����B
 * @return OdResult
 */
OdResult LoadRuntimeLibrary();

/**
 * @brief ���C�u�������A�����[�h����B
 */
void UnloadRuntimeLibrary();

/**
 * @brief ���b�Z�[�W"Olfactory Device"��Ԃ�
 * @param[in]  size msg_buf�̃T�C�Y
 * @param[out] msg_buf  ���b�Z�[�W�̕�������i�[����o�b�t�@�B���炩���߃��������m�ۂ��邱��
 * @return OdResult
 */
OdResult GetMessage(const uint32_t size, char* msg_buf);

}  // namespace sony::olfactory_device
