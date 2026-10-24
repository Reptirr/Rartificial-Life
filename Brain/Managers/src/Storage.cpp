#include <Axon.h>
#include <Dendrite.h>
#include <Neuron.h>
#include <ranges>
#include <Storage.h>
#include <Synaps.h>
#include <UUIDGenerator.h>


template<typename T>
std::string Storage::putImpl(std::unordered_map<std::string, T *> &map, T *item) {
    auto uuid = generateUuid();
    map.insert({uuid, item});
    return uuid;
}

template<typename T, typename Self>
auto & Storage::mapFor(Self &self) {
    if constexpr (std::is_same_v<T, Neuron>)        return self.neurons_;
    else if constexpr (std::is_same_v<T, Axon>)     return self.axons_;
    else if constexpr (std::is_same_v<T, Synaps>)   return self.synapses_;
    else if constexpr (std::is_same_v<T, Dendrite>) return self.dendrites_;
    else static_assert(always_false<T>, "Unknown type");
}

template<typename Map>
std::string Storage::randomKey(const Map &m) {
    if (m.empty()) return "";

    static std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<size_t> dist(0, m.size() - 1);

    return std::next(m.begin(), dist(rng))->first;
}

template<typename T>
T * Storage::get(const std::string &uuid) const {
    const auto &map = mapFor<T>(*this);
    auto it = map.find(uuid);
    return it != map.end() ? it->second : nullptr;
}

template<typename T>
bool Storage::remove(const std::string &uuid) {
    return mapFor<T>(*this).erase(uuid) > 0;
}

std::string Storage::put(Neuron *neuron) {
    return putImpl(neurons_, neuron);
}

std::string Storage::put(Axon *axon) {
    return putImpl(axons_, axon);
}

std::string Storage::put(Synaps *synaps) {
    return putImpl(synapses_, synaps);
}

std::string Storage::put(Dendrite *dendrite) {
    return putImpl(dendrites_, dendrite);
}

std::string Storage::getRandomNeuron() const { return randomKey(neurons_); }

std::string Storage::getRandomAxon() const { return randomKey(axons_); }

std::string Storage::getRandomDendrite() const { return randomKey(dendrites_); }

std::unordered_set<std::string> Storage::neurons() {
    auto k = std::ranges::views::keys(neurons_);
    return std::unordered_set(k.begin(), k.end());
}

std::unordered_set<std::string> Storage::synapses() {
    auto k = std::ranges::views::keys(synapses_);
    return std::unordered_set(k.begin(), k.end());
}

std::unordered_set<std::string> Storage::dendrites() {
    auto k = std::ranges::views::keys(dendrites_);
    return std::unordered_set(k.begin(), k.end());
}

BrainComponent * Storage::getBrainComponent(const std::string &uuid) const {
    if (auto *item = get<Neuron>(uuid))   return item;
    if (auto *item = get<Dendrite>(uuid)) return item;
    if (auto *item = get<Axon>(uuid))     return item;
    if (auto *item = get<Synaps>(uuid))   return item;

    return nullptr;
}
