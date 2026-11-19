#pragma once
#include <Experiment.h>
#include <GenomeRandomizer.h>
#include <Model.h>

class MainExperiment : public Experiment {
    int now_cell{4};
    double cell_size = 100/3.0; // meter

    int food_for_cell = cell_size*0.6; // 0.1 food for 1m; 1 food for 10m
    int food_i{};

    int tact_per_food = 1; // 1 еда в 1 такт, постепенно увеличивается

public:
    void preFirstTick(TickContext &ctx) override;
    void onTick(TickContext &ctx) override;
};
