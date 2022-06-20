//
// Created by zowlyfon on 6/20/22.
//

#ifndef O2_SPYSERVICEHELPERS_H
#define O2_SPYSERVICEHELPERS_H

#include "Framework/DeviceInfo.h"

#include <string>
#include <regex>

namespace o2::framework
{
class SpyServiceHelpers
{
 public:
  static bool parseHeader(std::string const& header, std::smatch& match);
  static void processHeader(DeviceInfo& info, std::smatch& match);
};
}

#endif // O2_SPYSERVICEHELPERS_H
