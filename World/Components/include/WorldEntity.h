#pragma once
#include <string>


class WorldController;

class WorldEntity {
protected:
    std::string this_id_;
    const SenseGetter *sense_{};

public:
    void init(const std::string &this_id, const SenseGetter *sense) {
        this_id_ = this_id;
        sense_ = sense;
    }

    std::string getId() {
        return this_id_;
    }

};
