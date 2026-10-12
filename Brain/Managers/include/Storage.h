#pragma once
#include <string>
#include <unordered_map>
#include <UUIDGenerator.h>


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
    std::string putImpl(std::unordered_map<std::string, T *> &map, T *item) {
        auto uuid = generateUuid();
        map.insert({uuid, item});
        return uuid;
    }

    template <typename T>
    static constexpr bool always_false = false;

    // Self выводится как const или не-const, так что одна функция работает для обоих случаев
    template <typename T, typename Self>
    static auto &mapFor(Self &self) {
        if constexpr (std::is_same_v<T, Neuron>)        return self.neurons_;
        else if constexpr (std::is_same_v<T, Axon>)     return self.axons_;
        else if constexpr (std::is_same_v<T, Synaps>)   return self.synapses_;
        else if constexpr (std::is_same_v<T, Dendrite>) return self.dendrites_;
        else static_assert(always_false<T>, "Unknown type");
    }

public:
    std::string put(Neuron *neuron) {
        return putImpl(neurons_, neuron);
    }

    std::string put(Axon *axon) {
        return putImpl(axons_, axon);
    }

    std::string put(Synaps *synaps) {
        return putImpl(synapses_, synaps);
    }

    std::string put(Dendrite *dendrite) {
        return putImpl(dendrites_, dendrite);
    }

    template <typename T>
    T *get(const std::string &uuid) const {
        const auto &map = mapFor<T>(*this);
        auto it = map.find(uuid);
        return it != map.end() ? it->second : nullptr;
    }

    template <typename T>
    bool remove(const std::string &uuid) {
        return mapFor<T>(*this).erase(uuid) > 0;
    }
};
