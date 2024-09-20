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

#pragma once

#include "olfactory_device_defs.h"

namespace sony::olfactory_device {

/**
 * @brief ライブラリをロードする。
 * @return OdResult
 */
OdResult LoadRuntimeLibrary();

/**
 * @brief ライブラリをアンロードする。
 */
void UnloadRuntimeLibrary();

/**
 * @brief 匂いの放出を開始する
 * @param[in] device_id デバイスのID
 * @param[in] scent_name 匂いの名称
 * @param[in] intensity 匂いの強度（例: 0〜100）
 * @return OdResult
 */
OdResult StartScentEmission(int device_id, const char* scent_name, int intensity);

/**
 * @brief 匂いの放出を停止する
 * @return OdResult
 */
OdResult StopScentEmission();

}  // namespace sony::olfactory_device
