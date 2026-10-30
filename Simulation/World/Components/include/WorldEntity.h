#pragma once
#include <MoveController.h>
#include <string>


class WorldController;

class WorldEntity {
protected:
    std::string this_id_;
    const SenseGetter *sense_getter_{};
    const MoveController *move_controller_{};

public:
    void init(const std::string &this_id, const SenseGetter *sense, const MoveController *move_controller) {
        this_id_ = this_id;
        sense_getter_ = sense;
        move_controller_ = move_controller;
    }

    std::string getId() {
        return this_id_;
    }

};
