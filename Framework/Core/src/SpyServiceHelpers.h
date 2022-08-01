// Copyright 2019-2022 CERN and copyright holders of ALICE O2.
// See https://alice-o2.web.cern.ch/copyright for details of the copyright holders.
// All rights not expressly granted are reserved.
//
// This software is distributed under the terms of the GNU General Public
// License v3 (GPL Version 3), copied verbatim in the file "COPYING".
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
#ifndef O2_FRAMEWORK_SPYSERVICEHELPERS_H
#define O2_FRAMEWORK_SPYSERVICEHELPERS_H

#include "Framework/DeviceInfo.h"

#include <string>
#include <regex>

namespace o2::header {
  class DataHeader;
}

namespace o2::framework
{
  class GuiRenderer;
  class HeaderType;
class SpyServiceHelpers
{
 public:
  static bool parseHeader(std::string const& header, std::smatch& match);
  static void processHeader(DeviceInfo& info, std::smatch& match);
  static bool parseData(std::string const& data, std::smatch& match);
  static void processData(DeviceInfo& info, std::smatch& match);
  static void webGUI(uv_timer_s* ctx, GuiRenderer* renderer, const o2::header::DataHeader* header, const std::string& data);
};
}

#endif // O2_FRAMEWORK_SPYSERVICEHELPERS_H
