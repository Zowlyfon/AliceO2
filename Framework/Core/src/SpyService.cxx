//
// Created by zowlyfon on 6/20/22.
//

#include "SpyService.h"
#include "Framework/DriverClient.h"
#include "Framework/DeviceSpec.h"
#include "Framework/DeviceState.h"
#include "Framework/ServiceRegistry.h"

#include <string>
#include <string_view>


namespace o2::framework {
SpyService::SpyService(ServiceRegistry& registry, DeviceState& deviceState)
  : mRegistry{registry},
    mDeviceState{deviceState},
    mDriverClient{registry.get<DriverClient>()}
{
}

void SpyService::sendHeader(std::string header)
{
  std::scoped_lock lock(mMutex);
  mDriverClient.tell(fmt::format("HEADER: {}", header));
  mDriverClient.flushPending();
}

} // namespace o2::framework