#ifndef METRIC_CHANGER_AUTOMATION_ACTION_H
#define METRIC_CHANGER_AUTOMATION_ACTION_H

#include "automation_action.h"
#include "benchmark_metric.h"

template<typename DerivedDrawer>
class MetricChangerAutomationAction : public AutomationAction
{
    using Parent = AutomationAction;

    bool actionDone = false;
    DerivedDrawer *benchmarkDrawer;
    std::shared_ptr<BenchmarkMetric> metric;

    public:

    MetricChangerAutomationAction(DerivedDrawer *drawer, const BenchmarkMetric &metric)
    : benchmarkDrawer{drawer},
    metric{metric.clone()}
    {};

    void start() override
    {
        Parent::start();
    };

    void doAction() override
    {
        if(actionDone)
        {
            return;
        }
        benchmarkDrawer->setMetric(*metric);
        actionDone = true;
        end();
    };

    void end() override
    {
        Parent::end();
    };

    std::shared_ptr<AutomationAction> clone() const & override
    {
        return std::make_shared<MetricChangerAutomationAction>(*this);
    };

    std::shared_ptr<AutomationAction> clone() && override
    {
        return std::make_shared<MetricChangerAutomationAction>(std::move(*this));
    };
};

#endif