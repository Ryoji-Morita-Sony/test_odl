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
//    std::cout << "[Log Level: " << static_cast<int>(level) << "] " << message << std::endl;
    std::cout << "[Log Level: " << static_cast<int>(level) << "] " << message;
  }
};

// Test case to start scent emission with float level
TEST_F(TestOlfactoryDevice, 01_start_scent_emission) {
  std::string device_id = "1";
  std::string scent_name = "0";
  float duration = 1.0f;

  // Register the custom log callback function
  OdResult result = sony::olfactory_device::sony_odRegisterLogCallback(TestOlfactoryDevice::CustomLogCallback);
  ASSERT_EQ(result, OdResult::SUCCESS);

  result = sony_odStartSession(device_id.c_str());
  ASSERT_EQ(result, OdResult::SUCCESS);
  std::this_thread::sleep_for(std::chrono::seconds(1));

  bool b_is_available = false;
  result = sony_odStartScentEmission(device_id.c_str(), scent_name.c_str(), duration, b_is_available);
  ASSERT_EQ(result, OdResult::SUCCESS);
  std::this_thread::sleep_for(std::chrono::seconds(1));

  result = sony_odEndSession(device_id.c_str());
  ASSERT_EQ(result, OdResult::SUCCESS);
}

// Test case to stop scent emission
TEST_F(TestOlfactoryDevice, 02_stop_scent_emission) {
  std::string device_id = "4";
  std::string scent_name = "1";
  float duration = 2.0f;

  // Register the custom log callback function
  OdResult result = sony::olfactory_device::sony_odRegisterLogCallback(TestOlfactoryDevice::CustomLogCallback);
  ASSERT_EQ(result, OdResult::SUCCESS);

  result = sony_odStartSession(device_id.c_str());
  ASSERT_EQ(result, OdResult::SUCCESS);

  bool b_is_available = false;
  result = sony_odStartScentEmission(device_id.c_str(), scent_name.c_str(), duration, b_is_available);
  ASSERT_EQ(result, OdResult::SUCCESS);
  std::this_thread::sleep_for(std::chrono::seconds(1));

  result = sony_odStopScentEmission(device_id.c_str());
  ASSERT_EQ(result, OdResult::SUCCESS);

  result = sony_odEndSession(device_id.c_str());
  ASSERT_EQ(result, OdResult::SUCCESS);
}

// Test case to handle the case where no session is active when stopping scent emission
TEST_F(TestOlfactoryDevice, 03_stop_scent_emission_without_session) {
  std::string device_id = "3";

  // Register the custom log callback function
  OdResult result = sony::olfactory_device::sony_odRegisterLogCallback(TestOlfactoryDevice::CustomLogCallback);
  ASSERT_EQ(result, OdResult::SUCCESS);

  result = sony_odStopScentEmission(device_id.c_str());
  ASSERT_EQ(result, OdResult::ERROR_UNKNOWN);
}

TEST_F(TestOlfactoryDevice, 04_is_scent_emission_available) {
  std::string device_id = "3";
  bool b_is_available = false;

  // Register the custom log callback function
  OdResult result = sony::olfactory_device::sony_odRegisterLogCallback(TestOlfactoryDevice::CustomLogCallback);
  ASSERT_EQ(result, OdResult::SUCCESS);

  result = sony_odStartSession(device_id.c_str());
  result = sony_odIsScentEmissionAvailable(device_id.c_str(), b_is_available);
  ASSERT_EQ(result, OdResult::SUCCESS);
  result = sony_odEndSession(device_id.c_str());
}

TEST_F(TestOlfactoryDevice, 05_is_scent_emission_available) {
  std::string device_id = "0";
  bool b_is_available = true;

  // Register the custom log callback function
  OdResult result = sony::olfactory_device::sony_odRegisterLogCallback(TestOlfactoryDevice::CustomLogCallback);
  ASSERT_EQ(result, OdResult::SUCCESS);

  result = sony_odStartSession("0");
  result = sony_odStartSession("1");

  int cnt = 0;
  while (cnt < 3) {
    result = sony_odStartScentEmission("0", "0", 1.0f, b_is_available);
    auto start = std::chrono::steady_clock::now();
    while (b_is_available == false) {
      result = sony_odIsScentEmissionAvailable("0", b_is_available);
      ASSERT_EQ(result, OdResult::SUCCESS);
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "[unit_test] end - start: " << elapsed_seconds.count() << "seconds" << std::endl;
    cnt++;
  }

  result = sony_odStopScentEmission("0");
  result = sony_odStopScentEmission("1");
  result = sony_odEndSession("0");
  result = sony_odEndSession("1");
}

// Test case to start scent emission with float level
TEST_F(TestOlfactoryDevice, 06_osc_devcies) {
  int max = 24;
  std::string device_id[24] = { "0",  "0",  "1",  "1",
                                "2",  "2",  "3",  "3",
                                "4",  "4",  "5",  "5",
                                "6",  "6",  "7",  "7",
                                "8",  "8",  "9",  "9",
                               "10", "10", "11", "11"
                              };
  std::string scent_name[2] = {"0", "1"};
  float duration = 1.0f;
  OdResult result;
  bool b_is_available = false;

  // Register the custom log callback function
  result = sony::olfactory_device::sony_odRegisterLogCallback(TestOlfactoryDevice::CustomLogCallback);
  ASSERT_EQ(result, OdResult::SUCCESS);

  //  result = sony_odStartScentEmission("3", "1", 3.0f, b_is_available);
  auto start = std::chrono::steady_clock::now();

  for (int i = 0; i < max; i++) {
    result = sony_odStartSession(device_id[i].c_str());
    ASSERT_EQ(result, OdResult::SUCCESS);
  }

  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  std::cout << "[unit_test] end - start: " << elapsed_seconds.count() << "seconds" << std::endl;

  for (int i = 0; i < max; i++) {
    while (b_is_available == false) {
      result = sony_odIsScentEmissionAvailable(device_id[i].c_str(), b_is_available);
      ASSERT_EQ(result, OdResult::SUCCESS);
    }
    result = sony_odStartScentEmission(device_id[i].c_str(), "0", duration, b_is_available);
    ASSERT_EQ(result, OdResult::SUCCESS);

    while (b_is_available == false) {
      result = sony_odIsScentEmissionAvailable(device_id[i].c_str(), b_is_available);
      ASSERT_EQ(result, OdResult::SUCCESS);
    }
    result = sony_odStartScentEmission(device_id[i].c_str(), "1", duration, b_is_available);
    ASSERT_EQ(result, OdResult::SUCCESS);

    result = sony_odStartScentEmission(device_id[i].c_str(), "0", duration, b_is_available);
    ASSERT_EQ(result, OdResult::SUCCESS);
  }

  for (int i = 0; i < max; i++) {
    result = sony_odStopScentEmission(device_id[i].c_str());
    ASSERT_EQ(result, OdResult::SUCCESS);
  }

  for (int i = 0; i < max; i++) {
    result = sony_odEndSession(device_id[i].c_str());
    ASSERT_EQ(result, OdResult::SUCCESS);
  }
}

}  // namespace
