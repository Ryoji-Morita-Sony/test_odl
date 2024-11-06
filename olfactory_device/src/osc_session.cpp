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
    : osc_ip_(""),
      osc_port_(OSC_PORT),
      connected_(false),
      t_flag_(false),
      t_wait_(THREAD_WAIT),
      t_cmd_("") {}

// Destructor
OscSession::~OscSession() {
  if (connected_) {
    Close();
  }
}

bool OscSession::Open(const char* device_id) {
  std::cout << "[OscSession] device_id: " << device_id << std::endl;
  osc_ip_ = device_id;
  connected_ = true;
  return true;
}

void OscSession::Close() {
  if (connected_) {
    connected_ = false;
    std::cout << "[OscSession] OSC connection closed." << std::endl;
  }
}

bool OscSession::IsConnected() const {
  return connected_;
}

bool OscSession::SendData(const std::string& data) {
  if (!connected_) {
    std::cerr << "[OscSession] OSC not connected." << std::endl;
    return false;
  }

  // Write data to OSC (platform-dependent)
  UdpTransmitSocket transmitSocket(IpEndpointName(osc_ip_.c_str(), OSC_PORT));

  char buffer[1024] = {0};
  osc::OutboundPacketStream p(buffer, sizeof(buffer) - 1);

  p << osc::BeginBundleImmediate << osc::BeginMessage("/scent") << data.c_str() << osc::EndMessage << osc::EndBundle;
  transmitSocket.Send(p.Data(), p.Size());

  std::cout << "[OscSession] Data sent: " << data << std::endl;
  return true;
}

bool OscSession::SendData(unsigned int data) {
  if (!connected_) {
    std::cerr << "[OscSession] OSC not connected." << std::endl;
    return false;
  }

  std::cout << "[OscSession] This function does nothing." << data << std::endl;
  return false;
}

bool OscSession::RecvData(std::string& data) {
  if (!connected_) {
    std::cerr << "[OscSession] OSC not connected." << std::endl;
    return false;
  }

  // Log the data being sent and simulate the sending operation
  std::cout << "[OscSession] Data recv: " << data << std::endl;
  return true;
}

void OscSession::ThreadFunc() {
  std::string result = "";

  while (t_flag_) {
    if (!t_cmd_._Equal("")) {
      if (!this->SendData(t_cmd_)) {
        std::cerr << "[OscSession] Failed to send." << std::endl;
      }
      if (!this->RecvData(result)) {
        std::cerr << "[OscSession] Failed to receive." << std::endl;
      }
      t_cmd_ = "";  // Set "" in a case of Scent.
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(t_wait_));
  }
  std::cout << "[OscSession] Thread ending..." << std::endl;
}

bool OscSession::StartThreadFunc() {
  if (!connected_) {
    std::cerr << "[OscSession] OSC not connected." << std::endl;
    return false;
  }

  t_flag_ = true;
  t_ = std::thread(&OscSession::ThreadFunc, this);
  std::cout << "[OscSession] Thread has started." << std::endl;
  return true;
}

bool OscSession::StopThreadFunc() {
  if (!connected_) {
    std::cerr << "[OscSession] OSC not connected." << std::endl;
    return false;
  }

  t_flag_ = false;
  if (t_.joinable()) {
    t_.join();
  }
  std::cout << "[OscSession] Thread has finished." << std::endl;
  return true;
}

bool OscSession::SendCmd(const std::string& cmd, long long wait) {
  if (!connected_) {
    std::cerr << "[OscSession] OSC not connected." << std::endl;
    return false;
  }

  t_cmd_ = cmd;
  t_wait_ = wait;
  return true;
}

bool OscSession::SetScent(unsigned int id, const std::string& name) {
  if (!connected_) {
    std::cerr << "[OscSession] OSC not connected." << std::endl;
    return false;
  }

  scent[id] = name;
  std::cout << id << ": " << scent[id] << std::endl;
  return true;
}

int OscSession::GetScent(const std::string& name) {
  if (!connected_) {
    std::cerr << "[OscSession] Error: Cannot get a scent, not connected to any device." << std::endl;
    return -1;
  }

  for (int i = 0; i < sizeof(scent); i++) {
    if (scent[i] == name) {
      return i;
    }
  }
  return -1;
}

}  // namespace sony::olfactory_device
