#pragma once
#include <BrainPoint.h>
#include <Uuid.h>

struct Genome;
class SubMap;
class DataCollector;
class BrainMap;

/// does not act on other components in destructor
class BrainComponent {
protected:
    BrainPoint point_{};
    Uuid this_id_{};
    BrainMap *brain_map_{};
    DataCollector *data_collector_{};

    virtual void onSetThisId() {}

public:
    /// set after all pre-creatings
    void setInit(const Uuid &this_id, BrainMap &brain_map, DataCollector *data_collector) {
        this_id_ = this_id;
        brain_map_ = &brain_map;
        data_collector_ = data_collector;
        onSetThisId();
    }

    virtual void handleProtein(SubMap &sub_map, const Genome &genome) = 0;

    double x() const { return point_.x; }
    double y() const { return point_.y; }

    void setX(const double x) { point_.x = x; }
    void setY(const double y) { point_.y = y; }

    BrainPoint point() const { return point_; }
    void setPoint(const BrainPoint point) { point_ = point; }

    // Get ID as Uuid
    const Uuid& getId() const { return this_id_; }
    // Get ID as string (for logging/UI)
    std::string getIdString() const { return this_id_.toString(); }

    virtual ~BrainComponent() = default;
};