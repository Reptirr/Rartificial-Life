#pragma once

#include <Genome.h>
#include <unordered_set>
#include <UUIDGenerator.h>


struct SubInfo;
class BrainComponent;
class Dendrite;
class Synaps;
class Axon;
class Neuron;

class Storage {
    Genome genome_;

    std::unordered_map<std::string, Neuron *> neurons_{};
    std::unordered_map<std::string, Axon *> axons_{};
    std::unordered_map<std::string, Synaps *> synapses_{};
    std::unordered_map<std::string, Dendrite *> dendrites_{};

    std::unordered_map<std::string, Protein *> subs_;

    template <typename T>
    std::string putImpl(std::unordered_map<std::string, T *> &map, T *item);

    template <typename T>
    static constexpr bool always_false = false;

    // Self выводится как const или не-const, так что одна функция работает для обоих случаев
    template <typename T, typename Self>
    static auto &mapFor(Self &self) {
        if constexpr (std::is_same_v<T, Neuron>)        return self.neurons_;
        else if constexpr (std::is_same_v<T, Axon>)     return self.axons_;
        else if constexpr (std::is_same_v<T, Synaps>)   return self.synapses_;
        else if constexpr (std::is_same_v<T, Dendrite>) return self.dendrites_;
        else if constexpr (std::is_same_v<T, Protein>)  return self.subs_;
        else static_assert(always_false<T>, "Unknown type");
    }

    template <typename Map>
    static std::string randomKey(const Map& m);

public:
    void setGenome(Genome genome) {
        genome_ = genome;
    }
    Genome &getGenome() {
        return genome_;
    }

    std::string put(Neuron *neuron);
    std::string put(Axon *axon);
    std::string put(Synaps *synaps);
    std::string put(Dendrite *dendrite);

    template <typename T>
    T *get(const std::string &uuid) const {
        const auto &map = mapFor<T>(*this);
        auto it = map.find(uuid);
        return it != map.end() ? it->second : nullptr;
    }

    template <typename T>
    bool remove(const std::string &uuid);

    std::string getRandomNeuron() const;
    std::string getRandomAxon() const;
    std::string getRandomDendrite() const;

    std::unordered_set<std::string> neurons();
    std::unordered_set<std::string> synapses();
    std::unordered_set<std::string> dendrites();

    BrainComponent *getBrainComponent(const std::string &uuid) const;

    // subs
    std::string putProtein(Protein *info) {
        auto id = generateUuid();
        subs_.insert({id, info});
        return id;
    }

    std::unordered_map<std::string, Protein *> &proteins();
};
