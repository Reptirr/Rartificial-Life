#pragma once
#include <QTimer>
#include <queue>
#include <UIController.h>
#include <unordered_map>

struct ToCreate {
    std::string id;
    std::unique_ptr<Data> data;
};

class UIBatcher {
    UIController &controller_;
    UIStorage &ui_storage_;

    std::queue<ToCreate> to_create_;
    std::unordered_map<std::string, std::unique_ptr<Data>> updates_;

public:
    explicit UIBatcher(UIController &controller, UIStorage &storage)
        : controller_(controller), ui_storage_(storage) {
    }

    void onCreate(const std::string &id, std::unique_ptr<Data> d) {
        to_create_.push({id, std::move(d)});
    }
    void onUpdate(std::string id, std::unique_ptr<Data> d) {
        updates_.insert_or_assign(id, std::move(d));
    }

    void applyBatch() {
        std::queue<ToCreate> to_create;
        std::unordered_map<std::string, std::unique_ptr<Data>> updates;

        to_create.swap(to_create_);
        updates.swap(updates_);

        while (!to_create.empty()) {
            auto creating = std::move(to_create.front());
            to_create.pop();

            auto id = std::move(creating.id);
            auto d = creating.data.get();

            if (const auto neuron = dynamic_cast<NeuronData*>(d)) {
                controller_.newNeuron(id, *neuron);
            } else if (const auto dendrite = dynamic_cast<DendriteData*>(d)) {
                controller_.newDendrite(id, *dendrite);
            } else if (const auto axon = dynamic_cast<AxonData*>(d)) {
                controller_.newAxon(id, *axon);
            } else if (const auto synapse = dynamic_cast<SynapsData*>(d)) {
                controller_.newSynaps(id, *synapse);
            } else {
                my_assert(false);
            }
        }

        for (auto& [component_id, data] : updates) {
            controller_.updateComponent(ui_storage_.getComponent(component_id), data.get());
        }
    }
};
