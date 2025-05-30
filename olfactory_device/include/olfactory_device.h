﻿/**
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
 * @brief Register a log callback function
 * @param[in] callback The callback function to be called with log messages and their levels
 * @return OdResult Returns SUCCESS if the callback is registered successfully, otherwise ERROR_UNKNOWN
 */
OLFACTORY_DEVICE_API OdResult sony_odRegisterLogCallback(OdLogCallback callback);

/**
 * @brief Start a session for the specified device
 * @param[in] device_id The UART port number (e.g., "COM3") representing the device
 * @return OdResult Returns SUCCESS if the session starts successfully, otherwise ERROR_UNKNOWN
 */
OLFACTORY_DEVICE_API OdResult sony_odStartSession(const char* device_id);

/**
 * @brief End the session for the specified device
 * @param[in] device_id The UART port number (e.g., "COM3") representing the device
 * @return OdResult Returns SUCCESS if the session ends successfully, otherwise ERROR_UNKNOWN
 */
OLFACTORY_DEVICE_API OdResult sony_odEndSession(const char* device_id);


/**
 * @brief Set the orientation of the scent emission for the specified device
 * @param[in] device_id The UART port number (e.g., "COM3") representing the device
 * @param[in] yaw The yaw angle of the scent emission in degrees
 * @param[in] pitch The pitch angle of the scent emission in degrees
 * @return OdResult Returns SUCCESS if the orientation is set successfully, otherwise ERROR_UNKNOWN
 */
OLFACTORY_DEVICE_API OdResult sony_odSetScentOrientation(const char* device_id, float yaw, float pitch);

/**
 * @brief Start scent emission for the specified device
 * @param[in] device_id The UART port number (e.g., "COM3") representing the device
 * @param[in] scent_name The name of the scent to emit
 * @param[in] duration The duration of emission
 * @param[out] is_available A boolean flag set to true if scent emission is available, false otherwise
 * @return OdResult Returns SUCCESS if the scent emission starts successfully, otherwise ERROR_UNKNOWN
 */
OLFACTORY_DEVICE_API OdResult sony_odStartScentEmission(const char* device_id, const char* scent_name,
                                                        float duration, bool& is_available);

/**
 * @brief Stop scent emission for the specified device
 * @param[in] device_id The UART port number (e.g., "COM3") representing the device
 * @return OdResult Returns SUCCESS if the scent emission stops successfully, otherwise ERROR_UNKNOWN
 */
OLFACTORY_DEVICE_API OdResult sony_odStopScentEmission(const char* device_id);

/**
 * @brief Check if scent emission is available for the specified device.
 * @param[in] device_id The UART port number (e.g., "COM3") representing the device
 * @param[out] is_available A boolean flag set to true if scent emission is available, false otherwise
 * @return OdResult Returns SUCCESS if the availability check is performed successfully, otherwise
 * ERROR_UNKNOWN
 */
OLFACTORY_DEVICE_API OdResult sony_odIsScentEmissionAvailable(const char* device_id, bool& is_available);

}  // namespace sony::olfactory_device
