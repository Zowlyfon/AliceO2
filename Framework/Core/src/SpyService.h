//
// Created by zowlyfon on 6/20/22.
//

#ifndef O2_SPYSERVICE_H
#define O2_SPYSERVICE_H

#include "Framework/ServiceHandle.h"
#include <mutex>

namespace o2::framework {
struct ServiceRegistry;
struct DeviceState;
struct DriverClient;

class SpyService
{
 public:
  SpyService(ServiceRegistry& registry, DeviceState& deviceState);

  /// Send header;
  void sendHeader(std::string header);

 private:
  bool mOnce = false;
  ServiceRegistry& mRegistry;
  DeviceState& mDeviceState;
  DriverClient& mDriverClient;
  std::mutex mMutex;
};
}


#endif // O2_SPYSERVICE_H
