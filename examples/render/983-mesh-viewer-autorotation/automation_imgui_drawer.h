#ifndef AUTOMATION_IMGUI_DRAWER_H
#define AUTOMATION_IMGUI_DRAWER_H

#include <imgui.h>
#include <vclib/render/drawers/plain_drawer.h>
#include <chrono>
#include <vector>
#include "automation_action.h"

template<typename DerivedDrawer>
class AutomationImguiDrawer : public vcl::PlainDrawer<DerivedDrawer>
{
    using NameActionPair = std::pair<std::string, AutomationAction>;
    std::vector<NameActionPair> automations;

public:

    void onDrawContent(uint viewId)
    {
        ImGuiIO& io = ImGui::GetIO();
        ImGui::Begin("Automation", nullptr);
        for(int i=0; i<automations.size(); i++){
            if(ImGui::Button(automations[i].first)){
                if(automations[i].second.isActive()){
                    automations[i].second.end()
                }else{
                    automations[i].second.start()
                }
            }
            if(automations[i].second.isActive()){
                automations[i].second.update();
            }
        }
        ImGui::End();
    }

    size_t addAutomation(const std::string& name, const std::function<void()> &fun)
    {
        automations.push_back(std::make_pair<NameFunPair::first_type, NameFunPair::second_type>(name, fun));
        return automations.size();
    }

    bool removeAutomationAt(size_t pos)
    {
        if(pos>=automations.size()){
            return false;
        }
        automations.erase(automations.begin() + pos);
        return true;
    }
};

#endif