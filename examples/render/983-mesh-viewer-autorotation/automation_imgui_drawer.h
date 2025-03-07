#include <imgui.h>
#include <vclib/render/drawers/plain_drawer.h>
#include <chrono>
#include <vector>

namespace vcl::imgui{

template<typename DerivedDrawer>
class AutomationImguiDrawer : public vcl::PlainDrawer<DerivedDrawer>
{
    std::chrono::time_point prev;
    using NameFunPair = std::pair<std::string,std::function<void()>>;
    std::vector<NameFunPair> automations;

public:

    void onDrawContent(uint viewId)
    {
        ImGuiIO& io = ImGui::GetIO();
        ImGui::Begin("Automation", nullptr);
        for(int i=0; i<automations.size(); i++){
            if(ImGui::Button(automations[i].first)){
                automations[i].second();
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

}