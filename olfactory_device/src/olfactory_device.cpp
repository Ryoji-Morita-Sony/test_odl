/**
 * Sony CONFIDENTIAL
 *
 * Copyright 2024 Sony Group Corporation
 *
 * DO NOT COPY AND/OR REDISTRIBUTE WITHOUT PERMISSION.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

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
