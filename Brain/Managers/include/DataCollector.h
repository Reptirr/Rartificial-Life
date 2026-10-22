#pragma once
#include <Storage.h>
#include <typedefs.h>

// ему говорят данные компоненты. а он уже вызывает колбеки, тем самым говоря ui о изменениях
class DataCollector {
    Storage *storage_;

    // consumers
    StringConsumer newNeuronConsumer;
    StringConsumer newSynapsConsumer;
    StringConsumer updateDendriteConsumer;
    StringConsumer updateAxonConsumer;

public:
    explicit DataCollector(
        Storage *storage
    ) : storage_(storage) {

    }

    // events
    void onNewNeuron(const std::string &neuron_id) const {
        if (newNeuronConsumer)
            newNeuronConsumer(neuron_id);
    }

    void onNewSynaps(const std::string &synaps_id) const {
        if (newSynapsConsumer)
            newSynapsConsumer(synaps_id);
    }

    void onUpdateDendrite(const std::string &dendrite_id) const {
        if (updateDendriteConsumer)
            updateDendriteConsumer(dendrite_id);
    }

    void onUpdateAxon(const std::string &axon_id) const {
        if (updateAxonConsumer)
            updateAxonConsumer(axon_id);
    }

    // setters
    void setNewNeuronConsumer(const StringConsumer &cons) {
        newNeuronConsumer = cons;
    }

    void setNewSynapsConsumer(const StringConsumer &cons) {
        newSynapsConsumer = cons;
    }

    void setUpdateDendriteConsumer(const StringConsumer &cons) {
        updateDendriteConsumer = cons;
    }

    void setUpdateAxonConsumer(const StringConsumer &cons) {
        updateAxonConsumer = cons;
    }
};