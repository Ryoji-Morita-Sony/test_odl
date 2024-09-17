#include "gtest/gtest.h"

#include "olfactory_device_api.h"
using namespace sony::training::hello_world;

#include <stdio.h>

namespace {

class TestHelloWorld : public ::testing::Test {
 protected:
  TestHelloWorld() {
  }

  virtual ~TestHelloWorld() {
  }

  virtual void SetUp() {
  }

  virtual void TearDown() {
  }
};

TEST_F(TestHelloWorld, 01_get_message) {
  char msg_buf[256];
  sony_training_hwGetMessage(256, msg_buf);
  printf("Recieved message is \"%s\".\n", msg_buf);
}

}