#pragma once
#include <AgentBodyConfig.h>
#include <AgentConfig.h>
#include <Experiment.h>
#include <QColor>
#include <QRect>


struct AgentDef {
    AgentBodyConfig agent_body_config;
    AgentConfig agent_config;
};

struct ObstacleDef {
    QPointF point;
    QRectF rect;
    QColor color;
};

struct SimulationDef {
    Experiment *experiment;

    int need_tps{100}; // -1 = неограниченно
    double dt{0.005}; // (s) физическое время в мире за 1 тик

};
