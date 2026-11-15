#pragma once

#include <BrainComponent.h>
#include <Storage.h>


class Storage;

class BrainMap {
    Storage* storage_;

public:
    explicit BrainMap(Storage *storage)
        : storage_(storage) {
    }

    std::vector<std::string> searchDendrite(BrainPoint from, double r);
};
