#pragma once
#include <typedefs.h>


// ему говорят данные компоненты. а он уже вызывает колбеки, тем самым говоря ui о изменениях
class DataCollector {
    // consumers
    StringConsumer newNeuronConsumer;
    StringConsumer newSynapsConsumer;
    StringConsumer updateConsumer;

public:
    // events
    void onNewNeuron(const std::string &neuron_id) const {
        if (newNeuronConsumer)
            newNeuronConsumer(neuron_id);
    }

    void onNewSynaps(const std::string &synaps_id) const {
        if (newSynapsConsumer)
            newSynapsConsumer(synaps_id);
    }

    void onUpdate(const std::string &id) const {
        if (updateConsumer)
            updateConsumer(id);
    }

    // setters
    void setNewNeuronConsumer(const StringConsumer &cons) {
        newNeuronConsumer = cons;
    }

    void setNewSynapsConsumer(const StringConsumer &cons) {
        newSynapsConsumer = cons;
    }

    void setUpdateConsumer(const StringConsumer &cons) {
        updateConsumer = cons;
    }
};