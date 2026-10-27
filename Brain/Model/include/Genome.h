#pragma once
#include <BrainPoint.h>
#include <Gen.h>
#include <map>

#define GEN_NAME inline const std::string

struct Protein {
    BrainPoint point{};
    double dist{};

    int genblock_i{-1};
};

struct Genome {
    std::vector<Protein> proteins{}; // белки: выбрасываются в сеть и при создании компонента он смотрит какие белки рядом, активируя блоки гена

    std::vector<GenBlock> genblocks{}; // блоки гена, (0) - дефолтный
};