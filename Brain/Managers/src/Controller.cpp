#include <Axon.h>
#include <Controller.h>
#include <Dendrite.h>
#include <Neuron.h>
#include <Synaps.h>

Controller::Controller(Storage &storage, Executor &executor, BrainMap &brain_map)
    : storage_(storage), executor_(executor), brain_map_(brain_map)
{

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

    return synaps_id;
}
