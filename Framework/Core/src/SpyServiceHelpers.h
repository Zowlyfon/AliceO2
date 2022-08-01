//
// Created by zowlyfon on 6/20/22.
//

#ifndef O2_SPYSERVICEHELPERS_H
#define O2_SPYSERVICEHELPERS_H

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

#endif // O2_SPYSERVICEHELPERS_H
