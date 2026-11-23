#pragma once

#include <string>
#include <WorldExecutor.h>
#include <Uuid.h>


class WorldDataCollector;
class SenseGetter;
class WorldController;

class WorldEntity {
protected:
    Uuid this_id_;
    const SenseGetter *sense_getter_{};
    const WorldExecutor *executor_{};
    WorldDataCollector *data_collector_{};

public:
    void init(const Uuid &this_id, const SenseGetter *sense, const WorldExecutor *executor, WorldDataCollector *data_collector) {
        this_id_ = this_id;
        sense_getter_ = sense;
        executor_ = executor;
        data_collector_ = data_collector;
    }

    Uuid getId() const {
        return this_id_;
    }
    std::string getIdString() const {
        return this_id_.toString();
    }

    virtual EntityKind kind() const = 0;

    virtual void onCollision(const WorldEntity *other) {}

    virtual ~WorldEntity() = default;
};
