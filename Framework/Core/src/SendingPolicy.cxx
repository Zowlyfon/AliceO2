// Copyright 2019-2020 CERN and copyright holders of ALICE O2.
// See https://alice-o2.web.cern.ch/copyright for details of the copyright holders.
// All rights not expressly granted are reserved.
//
// This software is distributed under the terms of the GNU General Public
// License v3 (GPL Version 3), copied verbatim in the file "COPYING".
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

#include "Framework/SendingPolicy.h"
#include "Framework/DeviceSpec.h"
#include "Framework/DataRefUtils.h"
#include "Framework/DataProcessingHeader.h"
#include "Headers/STFHeader.h"
#include "DeviceSpecHelpers.h"
#include <fairmq/Device.h>
#include "Headers/DataHeader.h"
#include "SpyService.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
namespace o2::framework
{

std::vector<SendingPolicy> SendingPolicy::createDefaultPolicies()
{
  return {SendingPolicy{
            .name = "dispatcher",
            .matcher = [](DeviceSpec const& spec, ConfigContext const&) { return spec.name == "Dispatcher" || DeviceSpecHelpers::hasLabel(spec, "Dispatcher"); },
            .send = [](FairMQDeviceProxy& proxy, fair::mq::Parts& parts, ChannelIndex channelIndex, ServiceRegistry& registry) {
              auto *channel = proxy.getOutputChannel(channelIndex);
              channel->Send(parts, -1); }},
          SendingPolicy{
            .name = "spy",
            .matcher = [](DeviceSpec const&, ConfigContext const&) { return true; },
            .send = [](FairMQDeviceProxy& proxy, fair::mq::Parts& parts, ChannelIndex channelIndex, ServiceRegistry& registry) {

              int i = 0;

              while (i < parts.Size()) {
                std::string headerString((char*)parts[i].GetData(), parts[i].GetSize());
                auto header = o2::header::get<o2::header::DataHeader*>(headerString.c_str());

                SpyGuiData spyGuiData;

                registry.get<SpyService>().sendHeader(headerString);
                spyGuiData.header = headerString;

                int payloadParts = (int)header->splitPayloadParts;

                int lastPart = i + payloadParts;

                while (i < lastPart) {
                  i++;
                  std::string dataString((char*)parts[i].GetData(), parts[i].GetSize());
                  registry.get<SpyService>().sendData(dataString, i);
                  spyGuiData.data.push_back(dataString);
                }

                registry.get<SpyService>().spyGuiData.push_back(spyGuiData);

                i++;
              }




              auto *channel = proxy.getOutputChannel(channelIndex);
              auto timeout = 1000;
              auto res = channel->Send(parts, timeout);
              if (res == (size_t)fair::mq::TransferCode::timeout) {
                LOGP(warning, "Timed out sending after {}s. Downstream backpressure detected on {}.", timeout/1000, channel->GetName());
                channel->Send(parts);
                LOGP(info, "Downstream backpressure on {} recovered.", channel->GetName());
              } else if (res == (size_t) fair::mq::TransferCode::error) {
                LOGP(fatal, "Error while sending on channel {}", channel->GetName());
              }
            }
          },
          SendingPolicy{
            .name = "default",
            .matcher = [](DeviceSpec const&, ConfigContext const&) { return true; },
            .send = [](FairMQDeviceProxy& proxy, fair::mq::Parts& parts, ChannelIndex channelIndex, ServiceRegistry& registry) {
              auto *channel = proxy.getOutputChannel(channelIndex);
              auto timeout = 1000;
              auto res = channel->Send(parts, timeout);
              if (res == (size_t)fair::mq::TransferCode::timeout) {
                LOGP(warning, "Timed out sending after {}s. Downstream backpressure detected on {}.", timeout/1000, channel->GetName());
                channel->Send(parts);
                LOGP(info, "Downstream backpressure on {} recovered.", channel->GetName());
              } else if (res == (size_t) fair::mq::TransferCode::error) {
                LOGP(fatal, "Error while sending on channel {}", channel->GetName());
              } }}};
}
} // namespace o2::framework
