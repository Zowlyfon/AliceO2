//
// Created by zowlyfon on 6/20/22.
//

#include <uv.h>
#include "SpyServiceHelpers.h"
#include "DebugGUI/imgui.h"
#include "GuiCallbackContext.h"
#include "SpyService.h"
#include "Framework/DeviceSpec.h"
#include "Framework/DeviceState.h"
#include "ControlWebSocketHandler.h"
#include "DriverServerContext.h"
#include "DPLWebSocket.h"
#include "Headers/DataHeader.h"


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

void SpyServiceHelpers::webGUI(uv_timer_s* ctx, GuiRenderer* renderer, const o2::header::DataHeader* header, const std::string& data) {
  if (header != nullptr) {
    ImGui::Begin(fmt::format("Spy {}", header->dataDescription.str).c_str());
    ImGui::CollapsingHeader("Actions", ImGuiTreeNodeFlags_DefaultOpen);
    if (ImGui::Button("Skip 10s")) {
      renderer->enableAfter = uv_now(ctx->loop) + 10000;
      uv_stop(ctx->loop);
    }
    if (ImGui::Button("Previous Frame")) {
      renderer->handler->mServerContext->registry->get<SpyService>().selectedFrame--;
    }
    if (ImGui::Button("Next Frame")) {
      renderer->handler->mServerContext->registry->get<SpyService>().selectedFrame++;
    }
    if (ImGui::CollapsingHeader("Header", ImGuiTreeNodeFlags_DefaultOpen)) {
      ImGui::Columns(2);
      ImGui::Text("sMagicString: %d", header->sMagicString);
      ImGui::Text("sVersion: %d", header->sVersion);
      ImGui::Text("sHeaderType: %s", header->sHeaderType.str);
      ImGui::Text("sSerializationMethod: %s", header->sSerializationMethod.str);
      ImGui::NextColumn();
      ImGui::Text("Header size: %d", header->headerSize);
      ImGui::Text("Payload Size: %d", header->payloadSize);
      ImGui::Text("Header Version: %d", header->headerVersion);
      ImGui::Text("flagsNextHeader: %d", header->flagsNextHeader);
      ImGui::Text("serialization: %s", header->serialization.str);
      ImGui::Text("Data Description: %s", header->dataDescription.str);
      ImGui::Text("Data Origin: %s", header->dataOrigin.str);
      ImGui::Text("Run Number: %d", header->runNumber);
      ImGui::Text("Sub Specification: %s", header->subSpecification);

      ImGui::Columns(1);
    }

    if (ImGui::CollapsingHeader("Payload", ImGuiTreeNodeFlags_DefaultOpen)) {
      ImGui::BeginChild("##ScrollingRegion", ImVec2(0, 430), false, ImGuiWindowFlags_HorizontalScrollbar);

      if (data != nullptr && !data.empty() && ImGui::BeginTable("Data", 5, ImGuiTableFlags_Borders)) {
        ImGui::TableSetupColumn("");
        ImGui::TableSetupColumn("#0");
        ImGui::TableSetupColumn("#1");
        ImGui::TableSetupColumn("#2");
        ImGui::TableSetupColumn("#3");
        ImGui::TableHeadersRow();
        for (int row = 0; row < data.length() / 4; row++) {
          ImGui::TableNextRow();
          ImGui::TableSetColumnIndex(0);
          ImGui::Text("%04d", row * 4);

          for (int column = 1; column < 5; column++) {
            std::stringstream hex;
            if (data.length() > row * 4) {
              ImGui::TableSetColumnIndex(column);
              unsigned char dataElem = data[row * 4 + (column - 1)];
              hex << std::hex << (int)dataElem;
            }
            std::string hexStr = hex.str();
            if (hexStr.length() == 1) {
              hexStr.insert(0, "0");
            }
            ImGui::Text("%s", hexStr.c_str());
          }
        }
        ImGui::EndTable();
      }
      ImGui::EndChild();
    }

    ImGui::End();
  }
}

} // namespace o2::framework