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

/**
 * @def OLFACTORY_DEVICE_API
 * DLLライブラリのエクスポート/インポートキーワードを定義する。
 */
#ifndef OLFACTORY_DEVICE_API
#ifdef _WIN64
#ifdef BUILD_DLL
#define OLFACTORY_DEVICE_API extern "C" __declspec(dllexport)
#else
#define OLFACTORY_DEVICE_API extern "C" __declspec(dllimport)
#endif
#else
#define OLFACTORY_DEVICE_API
#endif
#endif

namespace sony::olfactory_device {

/**
 * @brief 匂いの放出を開始する
 * @param[in] device_id デバイスのID
 * @param[in] scent_name 匂いの名称
 * @param[in] intensity 匂いの強度（例: 0〜100）
 * @return OdResult
 */
OLFACTORY_DEVICE_API OdResult sony_odStartScentEmission(int device_id, const char* scent_name, int intensity);

/**
 * @brief 芳香の放出を停止する
 * @return OdResult
 */
OLFACTORY_DEVICE_API OdResult sony_odStopScentEmission();

}  // namespace sony::olfactory_device
