#pragma once

#include "hello_world_api_defs.h"

namespace sony::training::hello_world {

/**
 * @brief ���C�u���������[�h����B
 * @return HwResult
 */
HwResult LoadRuntimeLibrary();

/**
 * @brief ���C�u�������A�����[�h����B
 */
void UnloadRuntimeLibrary();

/**
 * @brief ���b�Z�[�W"Hello World"��Ԃ�
 * @param[in]  size msg_buf�̃T�C�Y
 * @param[out] msg_buf  ���b�Z�[�W�̕�������i�[����o�b�t�@�B���炩���߃��������m�ۂ��邱��
 * @return HelloWorldResult
 */
HwResult GetMessage(const uint32_t size, char* msg_buf);

}
