#pragma once
#include <BrainPoint.h>
#include <Gen.h>
#include <Uuid.h>
#include <map>


struct Protein {
    Uuid id;

    BrainPoint point;
    double dist{};
    int neurons_num{};

    int genblock_i{-1};
};

struct Genome {
    // cash
    int proteins_num{};

    std::vector<Protein> proteins; // белки: выбрасываются в сеть и при создании компонента он смотрит какие белки рядом, активируя блоки гена

    std::vector<GenBlock> genblocks; // блоки гена, (0) - дефолтный
};