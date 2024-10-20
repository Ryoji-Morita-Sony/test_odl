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

#include "osc_session.h"

#include <iostream>
#include <iomanip> // for std::setw, std::setfill

namespace sony::olfactory_device {

// Constructor
OscSession::OscSession()
    : 
      connected_(false),
      t_flag_(false),
      t_wait_(THREAD_SCENT_WAIT),
      t_scent_(""),
      t_fan_("") {}

// Destructor
OscSession::~OscSession() {
  if (connected_) {
    Close();
  }
}

bool OscSession::Open(const char* device_id) {
  // Simulate opening a session and log the action
  std::cout << "[OscSession] Open called with device_id (port_num): " << device_id << std::endl;
//  std::cout << "[OscSession] If this were a real session, we would attempt to open a UART connection to port: " << device_id << std::endl;

  connected_ = true;  // Simulate a successful connection
  return true;
}

void OscSession::Close() {
  if (connected_) {
    // Log the action of closing the session
    std::cout << "[OscSession] Close called." << std::endl;
//    std::cout << "[OscSession] If this were a real session, we would close the UART connection here." << std::endl;

    connected_ = false;  // Simulate closing the connection
  }
}

bool OscSession::IsConnected() const {
  // Log the check and return the simulated connection status
  std::cout << "[OscSession] IsConnected called." << std::endl;
//  std::cout << "[OscSession] If this were a real session, we would check if the UART connection is active." << std::endl;

  return connected_;
}

bool OscSession::SendData(const std::string& data) {
  if (!connected_) {
    std::cerr << "[OscSession] Error: Cannot send data, not connected to any device." << std::endl;
    return false;
  }

  // Log the data being sent and simulate the sending operation
  std::cout << "[OscSession] SendData called with data: " << data << std::endl;
//  std::cout << "[OscSession] If this were a real session, we would send the data over the UART connection." << std::endl;

  return true;  // Simulate successful data transmission
}

bool OscSession::SendData(unsigned int data) {
  if (!connected_) {
    std::cerr << "[OscSession] Error: Cannot send data, not connected to any device." << std::endl;
    return false;
  }

  // Log the data being sent and simulate the sending operation
  std::cout << "[OscSession] SendData called with data: " << data << std::endl;
//  std::cout << "[OscSession] If this were a real session, we would send the data over the UART connection." << std::endl;

  return true;  // Simulate successful data transmission
}

bool OscSession::RecvData(std::string& data) {
  if (!connected_) {
    std::cerr << "[OscSession] Error: Cannot receive data, not connected to any device." << std::endl;
    return false;
  }

  // Log the data being sent and simulate the sending operation
  std::cout << "[OscSession] RecvData called, no data received because of a simulate." << std::endl;
//  std::cout << "[OscSession] If this were a real session, we would receive the data over the UART connection." << std::endl;

  return true;  // Simulate successful data transmission
}

void OscSession::ThreadFunc() {
  std::string result = "";

  while (t_flag_) {
    if (!t_scent_._Equal("")) {
      if (!this->SendData(t_scent_)) {
        std::cerr << "[OscSession] Failed to send a command on port: " << std::endl;
      }
      if (!this->RecvData(result)) {
        std::cerr << "[OscSession] Failed to receive result on port: " << std::endl;
      }
    }

    if (!t_fan_._Equal("")) {
      if (!this->SendData(t_fan_)) {
        std::cerr << "[OscSession] Failed to send a command on port: " << std::endl;
      }
      if (!this->RecvData(result)) {
        std::cerr << "[OscSession] Failed to receive result on port: " << std::endl;
      }
      t_fan_ = "";  // Set "" in a case of FAN.
    }

    std::this_thread::sleep_for(std::chrono::seconds(t_wait_));
  }
  std::cout << "[OscSession] Thread ending..." << std::endl;
}

bool OscSession::StartThreadFunc() {
  if (!connected_) {
    std::cerr << "[OscSession] Error: Cannot start a thread, not connected to any device." << std::endl;
    return false;
  }

  t_flag_ = true;
  t_ = std::thread(&OscSession::ThreadFunc, this);
  std::cout << "[OscSession] Thread has started." << std::endl;
  return true;
}

bool OscSession::StopThreadFunc() {
  if (!connected_) {
    std::cerr << "[OscSession] Error: Cannot stop a thread, not connected to any device." << std::endl;
    return false;
  }

  t_flag_ = false;
  if (t_.joinable()) {
    t_.join();
  }
  std::cout << "[OscSession] Thread has finished." << std::endl;
  return true;
}

bool OscSession::SetScent(const std::string& cmd, long long wait) {
  if (!connected_) {
    std::cerr << "[OscSession] Error: Cannot set a scent-command, not connected to any device." << std::endl;
    return false;
  }

  t_scent_ = cmd;
  t_wait_ = wait;
  return true;
}

bool OscSession::SetFan(const std::string& cmd, long long wait) {
  if (!connected_) {
    std::cerr << "[OscSession] Error: Cannot set a fan-command, not connected to any device." << std::endl;
    return false;
  }

  t_fan_ = cmd;
  t_wait_ = wait;
  return true;
}

}  // namespace sony::olfactory_device
