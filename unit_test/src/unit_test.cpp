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

namespace {

class TestOlfactoryDevice : public ::testing::Test {
 protected:
  TestOlfactoryDevice() {}

  virtual ~TestOlfactoryDevice() {}

  virtual void SetUp() {}

  virtual void TearDown() {}
};

// Test case to start scent emission with float level
TEST_F(TestOlfactoryDevice, 01_start_scent_emission) {
  // First, start a session for device "COM3"
  OdResult result = sony_odStartSession("COM3");
  ASSERT_EQ(result, OdResult::SUCCESS);

  // Then, start scent emission for device "COM3", with scent "Lavender" and level 0.75
  result = sony_odStartScentEmission("COM3", "Lavender", 0.75f);
  ASSERT_EQ(result, OdResult::SUCCESS);

  // End the session for device "COM3"
  result = sony_odEndSession("COM3");
  ASSERT_EQ(result, OdResult::SUCCESS);
}

// Test case to stop scent emission
TEST_F(TestOlfactoryDevice, 02_stop_scent_emission) {
  // First, start a session for device "COM3"
  OdResult result = sony_odStartSession("COM3");
  ASSERT_EQ(result, OdResult::SUCCESS);

  // Start scent emission for device "COM3"
  result = sony_odStartScentEmission("COM3", "Lavender", 0.75f);
  ASSERT_EQ(result, OdResult::SUCCESS);

  // Stop the scent emission for device "COM3"
  result = sony_odStopScentEmission("COM3");
  ASSERT_EQ(result, OdResult::SUCCESS);

  // End the session for device "COM3"
  result = sony_odEndSession("COM3");
  ASSERT_EQ(result, OdResult::SUCCESS);
}

// Test case to handle the case where no session is active when stopping scent emission
TEST_F(TestOlfactoryDevice, 03_stop_scent_emission_without_session) {
  // Attempt to stop scent emission without starting a session
  OdResult result = sony_odStopScentEmission("COM3");
  ASSERT_EQ(result, OdResult::ERROR_UNKNOWN);
}

// Test case to set the scent orientation
TEST_F(TestOlfactoryDevice, 04_set_scent_orientation) {
  // First, start a session for device "COM3"
  OdResult result = sony_odStartSession("COM3");
  ASSERT_EQ(result, OdResult::SUCCESS);

  // Set the scent orientation with yaw 45.0 and pitch 30.0
  result = sony_odSetScentOrientation("COM3", 45.0f, 30.0f);
  ASSERT_EQ(result, OdResult::SUCCESS);

  // End the session for device "COM3"
  result = sony_odEndSession("COM3");
  ASSERT_EQ(result, OdResult::SUCCESS);
}

}  // namespace
