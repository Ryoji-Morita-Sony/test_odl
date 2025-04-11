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
      connected_(false) {}

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
    connected_ = false;  // Simulate closing the connection
  }
}

bool StubSession::IsConnected() const {
  // Log the check and return the simulated connection status
  // spdlog::debug("[StubSession] IsConnected called.");
  // Comment out because of unit_test, sony_odIsScentEmissionAvailable() in while loop.

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

bool StubSession::RecvData(std::string& data) {
  if (!connected_) {
    spdlog::error("[StubSession] Error: Cannot receive data, not connected to any device.");
    return false;
  }

  // Log the data being sent and simulate the sending operation
  spdlog::debug("[StubSession] Data recv: no data because of a simulate.");

  return true;  // Simulate successful data transmission
}

bool StubSession::IsScentEmissionAvailable() {
  if (!connected_) {
    spdlog::error("[StubSession] Error: Cannot check available, not connected to any device.");
    return false;
  }

  spdlog::error("[StubSession] Hello World.");
  return true;
}

}  // namespace sony::olfactory_device
