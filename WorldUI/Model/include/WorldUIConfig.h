#pragma once

struct WorldUIConfig {
    static constexpr int kArcSteps = 30;

    // world obj
    static constexpr double world_obj_pen_w = 0;

    // agent
    static constexpr double agent_angle_head_size = 1;
    static constexpr double agent_angle_length = 15;
    static constexpr double agent_angle_w = 1;
    static constexpr QColor agent_angle_color = QColorConstants::Black;
    static constexpr double agent_font_size = 3;
};