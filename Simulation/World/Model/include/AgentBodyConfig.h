#pragma once

struct AgentBodyConfig {
    int rays{};
    double fov{}; // radian

    int getInputSize() {
        return rays * 3; // 3 colors per ray
    }
};