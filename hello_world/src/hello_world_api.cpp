#include "hello_world_api.h"

#include <string.h>

namespace sony::training::hello_world {

HELLO_WORLD_API HwResult sony_training_hwGetMessage(const uint32_t size, char* msg_buf) {
  strcpy_s(msg_buf, size, "Hello World");
  return HwResult::SUCCESS;
}

}
