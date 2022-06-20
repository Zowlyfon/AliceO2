//
// Created by zowlyfon on 6/20/22.
//

#include "SpyServiceHelpers.h"

namespace o2::framework {

bool SpyServiceHelpers::parseHeader(const std::string& header, std::smatch& match)
{
  const static std::regex headerRE(".*HEADER: (.*)", std::regex::optimize);
  return std::regex_search(header, match, headerRE);
}

void SpyServiceHelpers::processHeader(DeviceInfo& info, std::smatch& match)
{
  if (match.length() >= 1) {
    std::string header = match[1];
    info.header = header;
  }
}

} // namespace o2::framework