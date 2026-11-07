#pragma once
#include <string>
#include <WorldExecutor.h>


class SenseGetter;
class WorldController;

class WorldEntity {
protected:
    std::string this_id_;
    const SenseGetter *sense_getter_{};
    const WorldExecutor *executor_{};

public:
    void init(const std::string &this_id, const SenseGetter *sense, const WorldExecutor *executor) {
        this_id_ = this_id;
        sense_getter_ = sense;
        executor_ = executor;
    }

    std::string getId() const {
        return this_id_;
    }

    virtual EntityKind kind() const = 0;

    virtual void onCollision(const WorldEntity *other) {}

    virtual ~WorldEntity() = default;
};
