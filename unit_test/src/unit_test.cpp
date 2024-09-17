// Copyright (c) 2024 Sony Corporation
//
// This source code is unlicensed and all use is prohibited without the express permission of the copyright
// holder.
//
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.

#include "gtest/gtest.h"
#include "olfactory_device_api.h"
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

TEST_F(TestOlfactoryDevice, 01_start_scent_emission) {
  OdResult result = StartScentEmission(1, "Lavender", 75);  // �f�o�C�XID 1�ŁA���� "Lavender"�A���x75�ŊJ�n
  ASSERT_EQ(result, OdResult::SUCCESS);
}

TEST_F(TestOlfactoryDevice, 02_stop_scent_emission) {
  OdResult result = StopScentEmission();  // ���o��~
  ASSERT_EQ(result, OdResult::SUCCESS);
}

}  // namespace
