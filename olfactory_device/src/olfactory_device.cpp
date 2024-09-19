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

// �ÓI�I�u�W�F�N�g��UART��������
static UARTInitializer uart_initializer;  // ���C�u�������[�h����UART��������

OLFACTORY_DEVICE_API OdResult sony_odStartScentEmission(int device_id, const char* scent_name,
                                                        int intensity) {
  // �f�o�b�O���O�B�������i�񂾂�����Ă��������B
  printf("Scent emission started for device %d with scent \"%s\" and intensity: %d.\n", device_id, scent_name,
         intensity);

  // UART�R�}���h������
  char command[256];
  sprintf(command, "START %d %s %d\n", device_id, scent_name, intensity);

  // UART�R�}���h�𑗐M
  if (!uart_initializer.SendUARTCommand(command)) {
    return OdResult::ERROR_SEND_DEVICE_COMMAND_FAILED;
  }

  return OdResult::SUCCESS;
}

OLFACTORY_DEVICE_API OdResult sony_odStopScentEmission() {
  // �f�o�b�O���O�B�������i�񂾂�����Ă��������B
  printf("Scent emission stopped.\n");

  // UART�R�}���h������
  char command[256];
  sprintf(command, "STOP\n");

  // UART�R�}���h�𑗐M
  if (!uart_initializer.SendUARTCommand(command)) {
    return OdResult::ERROR_SEND_DEVICE_COMMAND_FAILED;
  }

  return OdResult::SUCCESS;
}

}  // namespace sony::olfactory_device
