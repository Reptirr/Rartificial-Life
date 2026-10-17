#pragma once
#include <BrainComponent.h>
#include <Storage.h>


class BrainMap {
    Storage& storage_;

public:
    explicit BrainMap(const int resolution) : cells_(resolution * resolution), resolution_(resolution) {

    }

    void insert(BrainPoint p) {

    }
};
