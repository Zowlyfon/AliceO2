//
// Created by zowlyfon on 6/20/22.
//

#ifndef O2_SPYSERVICE_H
#define O2_SPYSERVICE_H

#include "Framework/ServiceHandle.h"
#include <mutex>
#include <vector>
#include <fairmq/FwdDecls.h>

namespace o2::framework {
struct ServiceRegistry;
struct DeviceState;
struct DriverClient;

struct SpyGuiData {
  std::string header;
  std::vector<std::string> data;
};

struct GuiRenderer;

class SpyService
{
 public:
  SpyService(ServiceRegistry& registry, DeviceState& deviceState);

  /// Send header;
  void sendHeader(std::string header);

  /// Send data;
  void sendData(std::string data, int num);

  std::vector<SpyGuiData> spyGuiData;
  GuiRenderer* renderer;
  int selectedFrame = 0;
  int selectedData = 0;

 private:
  bool mOnce = false;
  ServiceRegistry& mRegistry;
  DeviceState& mDeviceState;
  DriverClient& mDriverClient;
  std::mutex mMutex;

};
}


#endif // O2_SPYSERVICE_H
