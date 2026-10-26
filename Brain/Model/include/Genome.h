#pragma once
#include <BrainPoint.h>
#include <map>

#define GEN_NAME inline const std::string

namespace GenNames {
    GEN_NAME NEURON_THRESHOLD = "neuron_threshold";
}

struct Protein {
    BrainPoint point{};
    double dist{};

    int genblock_i{-1};
};

struct Genome {
    std::vector<Protein> proteins{}; // белки: выбрасываются в сеть и при создании компонента он смотрит какие белки рядом, активируя блоки гена

    std::vector<std::map<std::string, std::string>> genblocks{}; // блоки гена
};