#pragma once
#include <BrainPoint.h>
#include <qpoint.h>

class BrainMap;

class BrainComponent {
protected:
    BrainPoint point_{};
    std::string this_id_{};
    BrainMap *brain_map_{};

    virtual void onSetThisId() {}

public:
    /// set after all pre-creatings
    void setInit(const std::string &this_id, BrainMap &brain_map) {
        this_id_ = this_id;
        brain_map_ = &brain_map;
        onSetThisId();
    }

    double x() const { return point_.x; }
    double y() const { return point_.y; }

    void setX(const double x) { point_.x = x; }
    void setY(const double y) { point_.y = y; }

    BrainPoint point() { return point_; }
    void setPoint(const BrainPoint point) { point_ = point; }

    virtual ~BrainComponent() = default;
};