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
    using Parent = vcl::PlainDrawer<DerivedDrawer>;
    using NameActionPair = std::pair<std::string, AutomationAction*>;
    std::vector<NameActionPair> automations;

public:

    using Parent::Parent;
    using Parent::onDraw;
    using Parent::onInit;
    using Parent::onResize;
    using Parent::onPostDraw;

    void onDrawContent(vcl::uint viewId) override
    {
        ImGuiIO& io = ImGui::GetIO();
        ImGui::Begin("Automation", nullptr);
        ImVec4 inactiveButtonCol = ImGui::GetStyleColorVec4(ImGuiCol_Button);
        ImVec4 activeButtonCol = ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive);
        for(int i=0; i<automations.size(); i++){
            bool changeStyleColor = automations[i].second->isActive();
            if(changeStyleColor){
                ImGui::PushStyleColor(ImGuiCol_Button, activeButtonCol);
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, inactiveButtonCol);
            }
            if(ImGui::Button(automations[i].first.c_str())){
                if(automations[i].second->isActive()){
                    automations[i].second->end();
                }else{
                    automations[i].second->start();
                }
            }
            if(automations[i].second->isActive()){
                automations[i].second->update();
            }
            if(changeStyleColor){
                ImGui::PopStyleColor(2);
            }
        }
        ImGui::End();
    }

    size_t addAutomation(const std::string& name, AutomationAction *action)
    {
        automations.push_back(std::pair<NameActionPair::first_type, NameActionPair::second_type>(name, action));
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