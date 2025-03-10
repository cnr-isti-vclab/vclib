#ifndef BENCHMARK_DRAWER_H
#define BENCHMARK_DRAWER_H

#include "automation_action.h"
#include <vector>

//una rotazione ogni tot frame; una rotazione ogni tot secondi
template<typename DerivedDrawer>
class BenchmarkDrawer : public vcl::PlainDrawer<DerivedDrawer>
{
    using Parent = vcl::PlainDrawer<DerivedDrawer>;
    std::vector<AutomationAction*> automations;
    size_t currentAutomationIndex = 0;
    bool firstCall = true;
    bool allDone = false;

public:

    using Parent::Parent;
    using Parent::onDraw;
    using Parent::onInit;
    using Parent::onResize;
    using Parent::onPostDraw;

    void onDrawContent(uint viewId) override
    {
        if(allDone){
            return;
        }
        if(firstCall){
            automations[0]->start();
            firstCall = false;
        }
        if(!automations[currentAutomationIndex]->isActive()){
            currentAutomationIndex++;
            if(allDone = (currentAutomationIndex >= automations.size())){
                return;
            }
            automations[currentAutomationIndex]->start();
        }
        automations[currentAutomationIndex]->update();
    };

    size_t addAutomation(AutomationAction *action)
    {
        automations.push_back(action);
        return automations.size()-1;
    }
};

#endif