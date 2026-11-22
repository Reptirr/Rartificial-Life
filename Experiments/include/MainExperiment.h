#pragma once
#include <Experiment.h>

class MainExperiment : public Experiment {
    int init_agents{1};

    int init_food{000};
    int min_food{00};

public:
    void preFirstTick(TickContext &ctx) override;
    void onTick(TickContext &ctx) override;
};
