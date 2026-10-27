#pragma once
#include <BrainPoint.h>
#include <SubMap.h>

class DataCollector;
class BrainMap;

class BrainComponent {
protected:
    BrainPoint point_{};
    std::string this_id_{};
    BrainMap *brain_map_{};
    DataCollector *data_collector_{};

    virtual void onSetThisId() {}

public:
    /// set after all pre-creatings
    void setInit(const std::string &this_id, BrainMap &brain_map, DataCollector *data_collector) {
        this_id_ = this_id;
        brain_map_ = &brain_map;
        data_collector_ = data_collector;
        onSetThisId();
    }

    virtual void handleProtein(const SubMap &sub_map, const Genome &genome) = 0;

    double x() const { return point_.x; }
    double y() const { return point_.y; }

    void setX(const double x) { point_.x = x; }
    void setY(const double y) { point_.y = y; }

    BrainPoint point() const { return point_; }
    void setPoint(const BrainPoint point) { point_ = point; }

    virtual ~BrainComponent() = default;
};