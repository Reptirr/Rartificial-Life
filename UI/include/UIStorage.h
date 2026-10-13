#pragma once

#include <set>
#include <string>

struct BrainLinkRef {
    std::string neuron1;
    std::string axon;
    std::string synaps;
    std::string dendrite;
    std::string neuron2;

    auto operator<=>(const BrainLinkRef&) const = default;
};


class UIStorage {
    std::set<BrainLinkRef> brain_links_{};

public:
    void addLink(const BrainLinkRef& link) {
        brain_links_.insert(link);
    }
    void removeLink(const BrainLinkRef& link) {
        brain_links_.erase(link);
    }
};