#include <Axon.h>
#include <Controller.h>
#include <Dendrite.h>
#include <Neuron.h>
#include <Synaps.h>

Controller::Controller(Storage &storage, Executor &executor, BrainMap &brain_map)
    : storage_(storage), executor_(executor), brain_map_(brain_map)
{

}

void Controller::setNewNeuronCallback(const std::function<void(std::string, std::string, std::string)> &new_neuron_callback) {
    new_neuron_callback_ = new_neuron_callback;
}

void Controller::setNewSynapsCallback(const std::function<void(std::string)> &new_synaps_callback) {
    new_synaps_callback_ = new_synaps_callback;
}

void Controller::setNewNodeAxonCallback(const std::function<void(std::string)> &new_node_axon_callback) {
    update_axon_callback_ = new_node_axon_callback;
}

std::unordered_map<std::string, std::string> Controller::createNeuron() {
    auto *neuron = new Neuron(&executor_);
    auto neuron_id = storage_.put(neuron);

    auto *axon = new Axon(&executor_, neuron_id);
    auto axon_id = storage_.put(axon);

    auto *dendrite = new Dendrite(&executor_, neuron_id);
    auto dendrite_id = storage_.put(dendrite);

    neuron->setInit(neuron_id, brain_map_);
    neuron->init(axon_id, dendrite_id);

    // random coordinates
    static std::random_device rd{};
    static std::mt19937 gen{rd()};
    static std::uniform_real_distribution dist{-1.0, 1.0};

    neuron->setPoint({ dist(gen), dist(gen) });
    axon->setPoint(neuron->point());
    dendrite->setPoint(neuron->point());

    dendrite->setInit(dendrite_id, brain_map_);
    neuron->setInit(neuron_id, brain_map_);
    axon->setInit(axon_id, brain_map_);

    new_neuron_callback_(neuron_id, dendrite_id, axon_id);

    return {
        {"neuron", neuron_id},
        {"axon", axon_id},
        {"dendrite", dendrite_id}
    };

}

std::string Controller::createSynaps(const std::string &dendrite_id, const std::string &axon_id, int axon_node_from) {
    auto *synaps = new Synaps(&executor_, dendrite_id, axon_id);
    auto synaps_id = storage_.put(synaps);

    auto *axon = storage_.get<Axon>(axon_id);
    auto *dendrite = storage_.get<Dendrite>(dendrite_id);

    axon->addSynaps(synaps_id, axon_node_from);
    dendrite->addSynaps(synaps_id);

    synaps->setInit(synaps_id, brain_map_);

    synaps->setPoint(middleBetween(axon->nodes().at(axon_node_from).point, dendrite->point()));

    new_synaps_callback_(synaps_id);

    return synaps_id;
}

void Controller::onNewAxonNode(const std::string &axon_id) const {
    update_axon_callback_(axon_id);
}
