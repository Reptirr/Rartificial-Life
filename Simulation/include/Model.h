#pragma once
#include <Genome.h>
#include <QRect>


struct AgentDef {
    QPointF point;

    AgentBodyConfig agent_body_config;
    AgentConfig agent_config;
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
