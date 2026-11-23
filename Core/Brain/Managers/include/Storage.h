#pragma once

#include <Genome.h>
#include <Neuron.h>
#include <unordered_set>
#include <UUIDGenerator.h>
#include <Uuid.h>


struct SubInfo;
class BrainComponent;
class Dendrite;
class Synaps;
class Axon;
class Neuron;

class Storage {
    Genome genome_;

    std::unordered_map<Uuid, Neuron *, UuidHash> neurons_{};
    std::unordered_map<Uuid, Axon *, UuidHash> axons_{};
    std::unordered_map<Uuid, Synaps *, UuidHash> synapses_{};
    std::unordered_map<Uuid, Dendrite *, UuidHash> dendrites_{};

    std::unordered_map<Uuid, Protein *, UuidHash> subs_;

    template <typename T>
    Uuid putImpl(std::unordered_map<Uuid, T *, UuidHash> &map, T *item);

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

public:
    void setGenome(Genome genome) {
        genome_ = genome;
    }
    Genome &getGenome() {
        return genome_;
    }

    Uuid put(Neuron *neuron);
    Uuid put(Axon *axon);
    Uuid put(Synaps *synaps);
    Uuid put(Dendrite *dendrite);

    template <typename T>
    T *get(const Uuid &uuid) const {
        const auto &map = mapFor<T>(*this);
        auto it = map.find(uuid);
        return it != map.end() ? it->second : nullptr;
    }

    template <typename T>
    bool remove(const Uuid &uuid);

    std::unordered_set<Uuid, UuidHash> neurons();
    std::unordered_set<Uuid, UuidHash> synapses();
    std::unordered_set<Uuid, UuidHash> dendrites();

    BrainComponent *getBrainComponent(const Uuid &uuid) const;

    // subs
    Uuid putProtein(Protein *info) {
        auto id = subs_.insert({info->id, info}).first->first;
        return id;
    }

    std::unordered_map<Uuid, Protein *, UuidHash> &proteins();

    ~Storage();
};
