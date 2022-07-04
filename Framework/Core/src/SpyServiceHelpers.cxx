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
bool SpyServiceHelpers::parseData(const std::string& data, std::smatch& match)
{
  const static std::regex dataRE(".*DATA: ([0-9]*), (.*)", std::regex::optimize);
  return std::regex_search(data, match, dataRE);
}
void SpyServiceHelpers::processData(DeviceInfo& info, std::smatch& match)
{
  if (match.length() >= 2) {
    int num = stoi(match[1]);
    std::string data = match[2];
    info.data = data;
  }
}

} // namespace o2::framework