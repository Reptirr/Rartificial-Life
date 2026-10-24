#pragma once

struct UIConfig {
    static constexpr double neuron_r = 14;
    static constexpr double synaps_r = 7;

    static constexpr double dendrite_width = 6;
    static constexpr double axon_width = 6;

    static constexpr double neuron_type_font_size = 6;
    static constexpr double synaps_info_font_size = 1.88;

    // Z VALUES
    static constexpr qreal synaps_z = 2;
    static constexpr qreal neuron_z = 1;
    static constexpr qreal dendrite_z = -1;
    static constexpr qreal axon_z = 0;
};