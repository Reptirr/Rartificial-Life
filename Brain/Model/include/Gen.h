#pragma once
#include <map>
#include <string>

using GenBlock = std::map<std::string, std::string>;

#define GENNAME inline const std::string

namespace Gen {
    // neuron
    GENNAME NEURON_THRESHOLD = "neuron_threshold";
    GENNAME NEURON_RANDOM_SPIKE_TICK = "neuron_random_spike_tick";
    GENNAME NEURON_REFRACTOR_TICK = "neuron_refractor_tick";

    // axon
    GENNAME AXON_ANGLE_DEVIATION = "axon_angle_deviation";
    GENNAME AXON_DEFAULT_STEP = "axon_default_step";
    GENNAME AXON_STEP_DEVIATION = "axon_step_deviation";
    GENNAME AXON_SEARCH_DENDRITE_R = "axon_search_dendrite_r";
    GENNAME AXON_MAX_NODES = "axon_max_nodes";

    // synaps
    GENNAME SYNAPS_W_REDUCE_LR = "synaps_w_reduce_lr";
    GENNAME SYNAPS_W_PRODUCE_LR = "synaps_w_produce_lr";
    GENNAME SYNAPS_W_MIN = "synaps_w_min";
    GENNAME SYNAPS_W_MAX = "synaps_w_max";
    GENNAME SYNAPS_W_INIT = "synaps_w_init";
    GENNAME SYNAPS_CA_DECAY = "synaps_ca_decay";
    GENNAME SYNAPS_MED_REDUCE_VAL = "synaps_med_reduce_val";
}


struct DefaultGenblock {
    inline static GenBlock get = {
        // neuron
        {Gen::NEURON_THRESHOLD,         "10.0"},
        {Gen::NEURON_RANDOM_SPIKE_TICK, "0"},     // 0 = шумовых спайков нет
        {Gen::NEURON_REFRACTOR_TICK,    "3"},

        // axon
        {Gen::AXON_ANGLE_DEVIATION,     "0.6"},   // разброс угла, рад
        {Gen::AXON_DEFAULT_STEP,        "0.07"},  // шаг роста по плоскости -1..1
        {Gen::AXON_STEP_DEVIATION,      "0.005"}, // разброс длины шага
        {Gen::AXON_SEARCH_DENDRITE_R,   "0.03"},  // радиус поиска дендрита
        {Gen::AXON_MAX_NODES,           "200"},   // потолок длины аксона

        // synaps
        {Gen::SYNAPS_W_REDUCE_LR,       "0.01"},
        {Gen::SYNAPS_W_PRODUCE_LR,      "0.01"},
        {Gen::SYNAPS_W_MIN,             "-1.0"},
        {Gen::SYNAPS_W_MAX,             "1.0"},
        {Gen::SYNAPS_W_INIT,            "1.0"},
        {Gen::SYNAPS_CA_DECAY,          "0.1"},
        {Gen::SYNAPS_MED_REDUCE_VAL,    "0.5"},
    };
};