#ifndef METRIC_CHANGER_AUTOMATION_ACTION_H
#define METRIC_CHANGER_AUTOMATION_ACTION_H

#include <vclib/render/automation/actions/abstract_automation_action.h>
#include <vclib/render/automation/metrics/benchmark_metric.h>

namespace vcl{

/*
    An automation that represents a change of metric in a BenchmarkDrawer.
*/
template<typename DerivedDrawer>
class MetricChangerAutomationAction : public AbstractAutomationAction
{
    using Parent = AbstractAutomationAction;

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
        benchmarkDrawer->setMetric(*metric);
        end();
    };

    void end() override
    {
        Parent::end();
    };

    std::shared_ptr<AbstractAutomationAction> clone() const & override
    {
        return std::make_shared<MetricChangerAutomationAction>(*this);
    };

    std::shared_ptr<AbstractAutomationAction> clone() && override
    {
        return std::make_shared<MetricChangerAutomationAction>(std::move(*this));
    };
};

}

#endif