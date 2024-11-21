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

#include "stub_session.h"

#include <iostream>
#include <iomanip> // for std::setw, std::setfill

// Third Party Libraries
#include <spdlog/spdlog.h>

namespace sony::olfactory_device {

// Constructor
StubSession::StubSession()
    : 
      connected_(false),
      t_flag_(false),
      t_wait_(THREAD_WAIT),
      t_cmd_("") {}

// Destructor
StubSession::~StubSession() {
  if (connected_) {
    Close();
  }
}

bool StubSession::Open(const char* device_id) {
  // Simulate opening a session and log the action
  spdlog::debug("[StubSession] Open called with device_id (port_num): {}", device_id);

  connected_ = true;  // Simulate a successful connection
  return true;
}

void StubSession::Close() {
  if (connected_) {
    // Log the action of closing the session
    spdlog::debug("[StubSession] Close called.");

    connected_ = false;  // Simulate closing the connection
  }
}

bool StubSession::IsConnected() const {
  // Log the check and return the simulated connection status
  spdlog::debug("[StubSession] IsConnected called.");

  return connected_;
}

bool StubSession::SendData(const std::string& data) {
  if (!connected_) {
    spdlog::error("[StubSession] Error: Cannot send data, not connected to any device.");
    return false;
  }

  // Log the data being sent and simulate the sending operation
  spdlog::debug("[StubSession] Data sent: no data because of a simulate: {}", data);

  return true;  // Simulate successful data transmission
}

bool StubSession::SendData(unsigned int data) {
  if (!connected_) {
    spdlog::error("[StubSession] Error: Cannot send data, not connected to any device.");
    return false;
  }

  // Log the data being sent and simulate the sending operation
  spdlog::debug("[StubSession] Data sent: no data because of a simulate: {}", data);

  return true;  // Simulate successful data transmission
}

bool StubSession::RecvData(std::string& data) {
  if (!connected_) {
    spdlog::error("[StubSession] Error: Cannot receive data, not connected to any device.");
    return false;
  }

  // Log the data being sent and simulate the sending operation
  spdlog::debug("[StubSession] Data recv: no data because of a simulate.");

  return true;  // Simulate successful data transmission
}

void StubSession::ThreadFunc() {
  std::string result = "";

  while (t_flag_) {
    if (!t_cmd_._Equal("")) {
      if (!this->SendData(t_cmd_)) {
        spdlog::error("[StubSession] Failed to send.");
      }
      if (!this->RecvData(result)) {
        spdlog::error("[StubSession] Failed to receive.");
      }
      t_cmd_ = "";
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(t_wait_));
  }
  spdlog::debug("[StubSession] Thread ending...");
}

bool StubSession::StartThreadFunc() {
  if (!connected_) {
    spdlog::error("[StubSession] Error: Cannot start a thread, not connected to any device.");
    return false;
  }

  t_flag_ = true;
  t_ = std::thread(&StubSession::ThreadFunc, this);
  spdlog::debug("[StubSession] Thread has started.");
  return true;
}

bool StubSession::StopThreadFunc() {
  if (!connected_) {
    spdlog::error("[StubSession] Error: Cannot stop a thread, not connected to any device.");
    return false;
  }

  t_flag_ = false;
  if (t_.joinable()) {
    t_.join();
  }
  spdlog::debug("[StubSession] Thread has finished.");
  return true;
}

bool StubSession::SendCmd(const std::string& cmd, long long wait) {
  if (!connected_) {
    spdlog::error("[StubSession] Error: Cannot send a command, not connected to any device.");
    return false;
  }

  t_cmd_ = cmd;
  t_wait_ = wait;
  return true;
}

bool StubSession::SetScent(unsigned int id, const std::string& name) {
  if (!connected_) {
    spdlog::error("[StubSession] Error: Cannot set a scent, not connected to any device.");
    return false;
  }

  scent[id] = name;
  return true;
}

int StubSession::GetScent(const std::string& name) {
  if (!connected_) {
    spdlog::error("[StubSession] Error: Cannot get a scent, not connected to any device.");
    return -1;
  }

  for (int i = 0; i < sizeof(scent); i++) {
    if (scent[i] == name) {
      return i;
    }
  }
  return -1;
}

bool StubSession::IsScentEmissionAvailable() {
  if (!connected_) {
    spdlog::error("[StubSession] Error: Cannot set a fan-command, not connected to any device.");
    return false;
  }

  spdlog::error("[StubSession] Hello World.");
  return true;
}

}  // namespace sony::olfactory_device
