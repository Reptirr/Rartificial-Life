#pragma once
#include <Genome.h>
#include <QRect>


struct AgentDef {
    Genome genome = {
        .proteins = {},
        .genblocks = {DefaultGenblock::get}
    };
    
    QPointF point;

    // common
    int num_of_common_neurons{};

    // vision
    int pixels{};
    double fov{};
    std::vector<BrainPoint> vision_neurons;
};

struct ObstacleDef {
    QPointF point;
    QRectF rect;
    QColor color;
};

struct SimulationDef {
    std::vector<AgentDef> init_agents;
    std::vector<ObstacleDef> obstacles;

    int need_tps{100}; // -1 = неограниченно
    double dt{0.005}; // (s) физическое время в мире за 1 тик

};
