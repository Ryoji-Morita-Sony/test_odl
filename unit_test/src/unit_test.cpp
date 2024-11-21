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
#include "olfactory_device_defs.h"
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

  // Log callback function
  static void CustomLogCallback(const char* message, OdLogLevel level) {
    std::cout << "[Log Level: " << static_cast<int>(level) << "] " << message << std::endl;
  }

};

// Test case to start scent emission with float level
TEST_F(TestOlfactoryDevice, 01_start_scent_emission) {
  std::string device_id = "1";
  std::string scent_name = "0";
  float duration = 1.0f;

  // Register the custom log callback function
  OdResult result =
      sony::olfactory_device::sony_odRegisterLogCallback(TestOlfactoryDevice::CustomLogCallback);
  ASSERT_EQ(result, OdResult::SUCCESS);

  // First, start a session for device
  result = sony_odStartSession(device_id.c_str());
  ASSERT_EQ(result, OdResult::SUCCESS);

  // wait
  std::this_thread::sleep_for(std::chrono::seconds(1));

  // Then, start scent emission for device with scent and level
  bool b_is_available = false;
  result = sony_odStartScentEmission(device_id.c_str(), scent_name.c_str(), duration, b_is_available);
  ASSERT_EQ(result, OdResult::SUCCESS);

  // wait
  std::this_thread::sleep_for(std::chrono::seconds(1));

  // End the session for device
  result = sony_odEndSession(device_id.c_str());
  ASSERT_EQ(result, OdResult::SUCCESS);
}

// Test case to stop scent emission
TEST_F(TestOlfactoryDevice, 02_stop_scent_emission) {
  std::string device_id = "2";
  std::string scent_name = "1";
  float duration = 2.0f;

  // First, start a session for device
  OdResult result = sony_odStartSession(device_id.c_str());
  ASSERT_EQ(result, OdResult::SUCCESS);

  // Start scent emission for device
  bool b_is_available = false;
  result = sony_odStartScentEmission(device_id.c_str(), scent_name.c_str(), duration, b_is_available);
  ASSERT_EQ(result, OdResult::SUCCESS);
  std::this_thread::sleep_for(std::chrono::seconds(1));
//Sleep(20000);   // milliseconds

  // Stop the scent emission for device
  result = sony_odStopScentEmission(device_id.c_str());
  ASSERT_EQ(result, OdResult::SUCCESS);
  std::this_thread::sleep_for(std::chrono::seconds(1));
  // Sleep(20000);   // milliseconds

  // End the session for device
  result = sony_odEndSession(device_id.c_str());
  ASSERT_EQ(result, OdResult::SUCCESS);
}

// Test case to handle the case where no session is active when stopping scent emission
TEST_F(TestOlfactoryDevice, 03_stop_scent_emission_without_session) {
  std::string device_id = "3";

  // Attempt to stop scent emission without starting a session
  OdResult result = sony_odStopScentEmission(device_id.c_str());
  ASSERT_EQ(result, OdResult::ERROR_UNKNOWN);
}

// Test case to start scent emission with float level
TEST_F(TestOlfactoryDevice, 04_osc_5_devcies) {
  int max = 5;
  std::string device_id[5] = {"0", "1", "2", "3", "4"};
  std::string scent_name[5] = {"0", "1", "0", "1", "0"};
  float duration[5] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
  OdResult result[5];

  for (int i = 0; i < max; i++) {
    result[i] = sony_odStartSession(device_id[i].c_str());
    ASSERT_EQ(result[i], OdResult::SUCCESS);
  }

  for (int i = 0; i < max; i++) {
    bool b_is_available = false;
    result[i] = sony_odStartScentEmission(device_id[i].c_str(), scent_name[i].c_str(), duration[i], b_is_available);
    ASSERT_EQ(result[i], OdResult::SUCCESS);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  for (int i = 0; i < max; i++) {
    result[i] = sony_odStopScentEmission(device_id[i].c_str());
    ASSERT_EQ(result[i], OdResult::SUCCESS);
  }

  for (int i = 0; i < max; i++) {
    result[i] = sony_odEndSession(device_id[i].c_str());
    ASSERT_EQ(result[i], OdResult::SUCCESS);
  }
}

}  // namespace
