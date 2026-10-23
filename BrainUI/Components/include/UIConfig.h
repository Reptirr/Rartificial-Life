#pragma once

struct UIConfig {
    static constexpr double neuron_r = 0.014;
    static constexpr double synaps_r = 0.007;

    static constexpr double dendrite_width = 0.006;
    static constexpr double axon_width = 0.006;

    // Z VALUES
    static constexpr qreal synaps_z = 2;
    static constexpr qreal neuron_z = 1;
    static constexpr qreal dendrite_z = -1;
    static constexpr qreal axon_z = 0;
};