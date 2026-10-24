#pragma once
#include <unordered_set>


class BrainComponent;
class Dendrite;
class Synaps;
class Axon;
class Neuron;

class Storage {
    std::unordered_map<std::string, Neuron *> neurons_{};
    std::unordered_map<std::string, Axon *> axons_{};
    std::unordered_map<std::string, Synaps *> synapses_{};
    std::unordered_map<std::string, Dendrite *> dendrites_{};

    template <typename T>
    std::string putImpl(std::unordered_map<std::string, T *> &map, T *item);

    template <typename T>
    static constexpr bool always_false = false;

    // Self выводится как const или не-const, так что одна функция работает для обоих случаев
    template <typename T, typename Self>
    static auto &mapFor(Self &self);

    template <typename Map>
    static std::string randomKey(const Map& m);

public:
    std::string put(Neuron *neuron);

    std::string put(Axon *axon);

    std::string put(Synaps *synaps);

    std::string put(Dendrite *dendrite);

    template <typename T>
    T *get(const std::string &uuid) const;

    template <typename T>
    bool remove(const std::string &uuid);

    std::string getRandomNeuron() const;
    std::string getRandomAxon() const;
    std::string getRandomDendrite() const;

    std::unordered_set<std::string> neurons();
    std::unordered_set<std::string> synapses();
    std::unordered_set<std::string> dendrites();

    BrainComponent *getBrainComponent(const std::string &uuid) const;
};
