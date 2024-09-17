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

TEST_F(TestOlfactoryDevice, 01_get_message) {
  char msg_buf[256];
  sony_odGetMessage(256, msg_buf);
  printf("Received message is \"%s\".\n", msg_buf);
}

}  // namespace
