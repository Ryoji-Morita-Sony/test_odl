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
#include <vector>
#include <sstream>

// Uncomment to be enabled Thread
//#define ENABLED_THREAD


namespace sony::olfactory_device {

// Constructor
OscSession::OscSession()
    : osc_ip_(""),
      osc_port_(OSC_PORT),
      connected_(false) {}

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

//  p << osc::BeginBundleImmediate << osc::BeginMessage("/scent") << data.c_str() << osc::EndMessage << osc::EndBundle;
  size_t start = data.find('(');
  size_t end = data.find(')');
  std::string command = data.substr(0, start);
  std::string arguments = data.substr(start + 1, end - start - 1);
  std::istringstream iss(arguments);
  std::string arg;
  std::vector<std::string> args;
  while (std::getline(iss, arg, ',')) {
    args.push_back(arg);
  }
  int target = std::stoi(args[0]);
  int level = std::stoi(args[1]);

  p << osc::BeginBundleImmediate << osc::BeginMessage("/scent") << command.c_str() << target << level << osc::EndMessage << osc::EndBundle;
  transmitSocket.Send(p.Data(), p.Size());

  std::cout << "[OscSession] Data sent: " << command << "/" << target << "/" << level << std::endl;
  return true;
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

bool OscSession::IsScentEmissionAvailable() {
  if (!connected_) {
    std::cerr << "[OscSession] OSC not connected." << std::endl;
    return false;
  }

  return true;
}

}  // namespace sony::olfactory_device
