#pragma once
#include <Experiment.h>

class MainExperiment : public Experiment {
    int init_agents{5};

    int init_food{2000};
    int min_food{300};

    int numbers{};

public:
    void preFirstTick(TickContext &ctx) override;
    void onTick(TickContext &ctx) override;
};
