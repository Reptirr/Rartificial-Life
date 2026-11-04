#pragma once

struct AgentBodyConfig {
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