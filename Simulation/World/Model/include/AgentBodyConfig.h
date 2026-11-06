#pragma once
#include <box2d/math_types.h>

struct AgentBodyConfig {
    b2Pos pos{};

    // ---------
    // INPUT
    // ---------
    int rays{};
    double fov{}; // radian

    // ---------
    // OUTPUT
    // ---------
    int move_dirs{};

};
