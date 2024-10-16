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

namespace sony::olfactory_device {

// Constructor
StubSession::StubSession() : connected_(false) {}

// Destructor
StubSession::~StubSession() {
  if (connected_) {
    Close();
  }
}

bool StubSession::Open(const char* device_id) {
  // Simulate opening a session and log the action
  std::cout << "[StubSession] Open called with device_id (port_num): " << device_id << std::endl;
  std::cout
      << "[StubSession] If this were a real session, we would attempt to open a UART connection to port: "
      << device_id << std::endl;

  connected_ = true;  // Simulate a successful connection
  return true;
}

void StubSession::Close() {
  if (connected_) {
    // Log the action of closing the session
    std::cout << "[StubSession] Close called." << std::endl;
    std::cout << "[StubSession] If this were a real session, we would close the UART connection here."
              << std::endl;

    connected_ = false;  // Simulate closing the connection
  }
}

bool StubSession::IsConnected() const {
  // Log the check and return the simulated connection status
  std::cout << "[StubSession] IsConnected called." << std::endl;
  std::cout << "[StubSession] If this were a real session, we would check if the UART connection is active."
            << std::endl;

  return connected_;
}

bool StubSession::SendData(const std::string& data) {
  if (!connected_) {
    std::cerr << "[StubSession] Error: Cannot send data, not connected to any device." << std::endl;
    return false;
  }

  // Log the data being sent and simulate the sending operation
  std::cout << "[StubSession] SendData called with data: " << data << std::endl;
  std::cout << "[StubSession] If this were a real session, we would send the data over the UART connection."
            << std::endl;

  return true;  // Simulate successful data transmission
}

bool StubSession::SendData(unsigned int data) {
  if (!connected_) {
    std::cerr << "[StubSession] Error: Cannot send data, not connected to any device." << std::endl;
    return false;
  }

  // Log the data being sent and simulate the sending operation
  std::cout << "[StubSession] SendData called with data: " << data << std::endl;
  std::cout << "[StubSession] If this were a real session, we would send the data over the UART connection."
            << std::endl;

  return true;  // Simulate successful data transmission
}

bool StubSession::RecvData(std::string& data) {
  if (!connected_) {
    std::cerr << "[StubSession] Error: Cannot receive data, not connected to any device." << std::endl;
    return false;
  }

  // Log the data being sent and simulate the sending operation
  std::cout << "[StubSession] RecvData called with data: " << data << std::endl;
  std::cout << "[StubSession] If this were a real session, we would receive the data over the UART connection."
            << std::endl;

  return true;  // Simulate successful data transmission
}

void StubSession::ThreadFunction() {
  std::cout << "[StubSession] ThreadFunction called." << std::endl;
}

void StubSession::StartThread(long long wait) {
  std::cout << "[StubSession] StartThread called with wait: " << wait << std::endl;
}

void StubSession::StopThread() {
  std::cout << "[StubSession] StopThread called." << std::endl;
}

void StubSession::SetData(const std::string& data) {
  std::cout << "[StubSession] SetData called with data: " << data << std::endl;
}

void StubSession::SetData(unsigned int data) {
  std::cout << "[StubSession] SetData called with data: " << data << std::endl;
}

}  // namespace sony::olfactory_device
