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

#include "gtest/gtest.h"
#include "olfactory_device.h"
using namespace sony::olfactory_device;

#include <stdio.h>
#include <thread>
#include <chrono>

#include <iostream>
#include <string>
#include <windows.h>

namespace {

class TestOlfactoryDevice : public ::testing::Test {
 protected:
  TestOlfactoryDevice() {}

  virtual ~TestOlfactoryDevice() {}

  virtual void SetUp() {}

  virtual void TearDown() {}

  // 自作の関数
  // std::wstring を const char* に変換する
  const char* ConvertWStringToConstChar(const std::wstring& wstr) {
    int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (bufferSize == 0) {
      return nullptr;
    }

    char* buffer = new char[bufferSize];
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, buffer, bufferSize, nullptr, nullptr);
    return buffer;
  }

};

// Test case to start scent emission with float level
TEST_F(TestOlfactoryDevice, DISABLED_01_start_scent_emission) {
  std::string device_id = "COM3";
  std::string name = "5";
  float level = 0.85f;

  // First, start a session for device
  OdResult result = sony_odStartSession(device_id.c_str());
  ASSERT_EQ(result, OdResult::SUCCESS);

  // Then, start scent emission for device with scent and level
  result = sony_odStartScentEmission(device_id.c_str(), name.c_str(), level);
  ASSERT_EQ(result, OdResult::SUCCESS);

  // End the session for device
  result = sony_odEndSession(device_id.c_str());
  ASSERT_EQ(result, OdResult::SUCCESS);
}

// Test case to stop scent emission
TEST_F(TestOlfactoryDevice, DISABLED_02_stop_scent_emission) {
  std::string device_id = "COM3";
  std::string name = "1";
  float level = 0.75f;

  // First, start a session for device
  OdResult result = sony_odStartSession(device_id.c_str());
  ASSERT_EQ(result, OdResult::SUCCESS);

  // Start scent emission for device
  result = sony_odStartScentEmission(device_id.c_str(), name.c_str(), level);
  ASSERT_EQ(result, OdResult::SUCCESS);

  // Stop the scent emission for device
  result = sony_odStopScentEmission(device_id.c_str());
  ASSERT_EQ(result, OdResult::SUCCESS);

  // End the session for device
  result = sony_odEndSession(device_id.c_str());
  ASSERT_EQ(result, OdResult::SUCCESS);
}

// Test case to handle the case where no session is active when stopping scent emission
TEST_F(TestOlfactoryDevice, DISABLED_03_stop_scent_emission_without_session) {
  std::string device_id = "COM3";

  // Attempt to stop scent emission without starting a session
  OdResult result = sony_odStopScentEmission(device_id.c_str());
  ASSERT_EQ(result, OdResult::ERROR_UNKNOWN);
}

// Test case to set the scent orientation
TEST_F(TestOlfactoryDevice, DISABLED_04_set_scent_orientation) {
  std::string device_id = "COM4";
  float yaw = 180.0f;
  float pitch = 0.0f;

  // First, start a session for device
  OdResult result = sony_odStartSession(device_id.c_str());
  ASSERT_EQ(result, OdResult::SUCCESS);

  // Set the scent orientation with yaw and pitch
  result = sony_odSetScentOrientation(device_id.c_str(), yaw, pitch);
  ASSERT_EQ(result, OdResult::SUCCESS);

  // End the session for device
  result = sony_odEndSession(device_id.c_str());
  ASSERT_EQ(result, OdResult::SUCCESS);
}

// Test case to set the scent orientation
TEST_F(TestOlfactoryDevice, DISABLED_05_set_scent_orientation_motion) {
  std::string device_id = "COM4";
  float yaw = 0.0f;
  float pitch = 0.0f;
  int cnt_yaw = 1;
  int cnt_pitch = 1;
  int time_start = 2;   // seccond
  int time_wait = 16;   // millsecond

  // First, start a session for device
  OdResult result = sony_odStartSession(device_id.c_str());
  ASSERT_EQ(result, OdResult::SUCCESS);

  // (yaw, pitch) = (0, 0)
  result = sony_odSetScentOrientation(device_id.c_str(), yaw, pitch);
  ASSERT_EQ(result, OdResult::SUCCESS);
  std::this_thread::sleep_for(std::chrono::seconds(time_start));

  for (int i = 0; i < 360; i++) {
    result = sony_odSetScentOrientation(device_id.c_str(), yaw, pitch);
    ASSERT_EQ(result, OdResult::SUCCESS);
    std::this_thread::sleep_for(std::chrono::milliseconds(time_wait));
    yaw = yaw + cnt_yaw;
    pitch = pitch + cnt_pitch;
    if (pitch > 90) {
      cnt_pitch = -1;
    } else if (pitch < -90) {
      cnt_pitch = 1;
    }
  }

  // (yaw, pitch) = (0, 0)
  yaw = 0;
  pitch = 0;
  result = sony_odSetScentOrientation(device_id.c_str(), yaw, pitch);
  ASSERT_EQ(result, OdResult::SUCCESS);
  std::this_thread::sleep_for(std::chrono::seconds(time_start));

  // End the session for device
  result = sony_odEndSession(device_id.c_str());
  ASSERT_EQ(result, OdResult::SUCCESS);
}

// Test case to start scent emission with float level
TEST_F(TestOlfactoryDevice, 06_uart_9_devcies) {
  int max = 9;
  std::wstring device_id[9] = {L"COM3", L"COM5", L"COM6", L"COM7", L"COM8", L"COM9",
                               L"\\\\.\\COM12", L"\\\\.\\COM13", L"\\\\.\\COM14"};
  std::string name[9] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};
  float level[9] = {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f};
  OdResult result[9];

  for (int i = 0; i < max; i++) {
    result[i] = sony_odStartSession(ConvertWStringToConstChar(device_id[i]));
    ASSERT_EQ(result[i], OdResult::SUCCESS);
  }

  while(true) {
    for (int i = 0; i < max; i++) {
      result[i] =
          sony_odStartScentEmission(ConvertWStringToConstChar(device_id[i]), name[i].c_str(), level[i]);
      ASSERT_EQ(result[i], OdResult::SUCCESS);
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  }

  for (int i = 0; i < max; i++) {
    result[i] = sony_odStopScentEmission(ConvertWStringToConstChar(device_id[i]));
    ASSERT_EQ(result[i], OdResult::SUCCESS);
  }

  for (int i = 0; i < max; i++) {
    result[i] = sony_odEndSession(ConvertWStringToConstChar(device_id[i]));
    ASSERT_EQ(result[i], OdResult::SUCCESS);
  }
}

}  // namespace
