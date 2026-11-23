#include <Axon.h>
#include <Dendrite.h>
#include <Neuron.h>
#include <ranges>
#include <Storage.h>
#include <Synaps.h>
#include <UUIDGenerator.h>


template<typename T>
Uuid Storage::putImpl(
    std::unordered_map<Uuid, T*, UuidHash>& map,
    T* item
) {
    auto uuid = generateUuid();
    map.emplace(uuid, item);
    return uuid;
}

template<typename T>
bool Storage::remove(const Uuid &uuid) {
    return mapFor<T>(*this).erase(uuid) > 0;
}

Uuid Storage::put(Neuron *neuron) {
    return putImpl(neurons_, neuron);
}

Uuid Storage::put(Axon *axon) {
    return putImpl(axons_, axon);
}

Uuid Storage::put(Synaps *synaps) {
    return putImpl(synapses_, synaps);
}

Uuid Storage::put(Dendrite *dendrite) {
    return putImpl(dendrites_, dendrite);
}

std::unordered_set<Uuid, UuidHash> Storage::neurons() {
    auto k = std::ranges::views::keys(neurons_);
    return std::unordered_set<Uuid, UuidHash>(k.begin(), k.end());
}

std::unordered_set<Uuid, UuidHash> Storage::synapses() {
    auto k = std::ranges::views::keys(synapses_);
    return std::unordered_set<Uuid, UuidHash>(k.begin(), k.end());
}

std::unordered_set<Uuid, UuidHash> Storage::dendrites() {
    auto k = std::ranges::views::keys(dendrites_);
    return std::unordered_set<Uuid, UuidHash>(k.begin(), k.end());
}

BrainComponent * Storage::getBrainComponent(const Uuid &uuid) const {
    if (auto *item = get<Neuron>(uuid))   return item;
    if (auto *item = get<Dendrite>(uuid)) return item;
    if (auto *item = get<Axon>(uuid))     return item;
    if (auto *item = get<Synaps>(uuid))   return item;

    return nullptr;
}

std::unordered_map<Uuid, Protein *, UuidHash> &Storage::proteins() {
    return subs_;
}

Storage::~Storage() {
    // удаляем тут все
    for (auto *neuron : neurons_ | std::views::values) {
        delete neuron;
    }
    for (auto *dendrite : dendrites_ | std::views::values) {
        delete dendrite;
    }
    for (auto *axon : axons_ | std::views::values) {
        delete axon;
    }
    for (auto *synaps : synapses_ | std::views::values) {
        delete synaps;
    }
    for (auto *protein : subs_ | std::views::values) {
        delete protein;
    }
}