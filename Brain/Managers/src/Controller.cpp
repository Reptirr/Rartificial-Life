#include <Axon.h>
#include <Controller.h>
#include <Dendrite.h>
#include <Neuron.h>
#include <Synaps.h>

Controller::Controller(Storage &storage, Executor &executor, BrainMap &brain_map, DataCollector &data_collector)
    : storage_(storage), executor_(executor), brain_map_(brain_map), data_collector_(data_collector) {
}

std::unordered_map<std::string, std::string> Controller::createCommonNeuron() {
    auto *neuron = new Neuron(&executor_, NeuronType::COMMON);
    auto neuron_id = storage_.put(neuron);

    auto *axon = new Axon(&executor_, neuron_id);
    auto axon_id = storage_.put(axon);

    auto *dendrite = new Dendrite(&executor_, neuron_id);
    auto dendrite_id = storage_.put(dendrite);

    // random coordinates
    static std::random_device rd{};
    static std::mt19937 gen{rd()};
    static std::uniform_real_distribution dist{-1.0, 1.0};

    neuron->setPoint({ dist(gen), dist(gen) });
    axon->setPoint(neuron->point());
    dendrite->setPoint(neuron->point());

    neuron->setInit(neuron_id, brain_map_, &data_collector_);
    neuron->init(axon_id, dendrite_id);

    dendrite->setInit(dendrite_id, brain_map_, &data_collector_);
    neuron->setInit(neuron_id, brain_map_, &data_collector_);
    axon->setInit(axon_id, brain_map_, &data_collector_);


    return {
        {"neuron", neuron_id},
        {"axon", axon_id},
        {"dendrite", dendrite_id}
    };
}

std::string Controller::createSynaps(const std::string &dendrite_id, const std::string &axon_id, int axon_node_from) {
    auto *axon = storage_.get<Axon>(axon_id);
    auto *dendrite = storage_.get<Dendrite>(dendrite_id);

    auto *synaps = new Synaps(&executor_, dendrite_id, axon_id, storage_.get<Neuron>(axon->getNeuron())->getType());
    auto synaps_id = storage_.put(synaps);


    synaps->setPoint(middleBetween(axon->nodes().at(axon_node_from).point, dendrite->point()));
    synaps->setInit(synaps_id, brain_map_, &data_collector_);

    axon->addSynaps(synaps_id, axon_node_from);
    dendrite->addSynaps(synaps_id);


    return synaps_id;
}

std::unordered_map<std::string, std::string> Controller::createInputNeuron(BrainPoint point) {
    auto *neuron = new Neuron(&executor_, NeuronType::INPUT);
    auto neuron_id = storage_.put(neuron);

    auto *axon = new Axon(&executor_, neuron_id);
    auto axon_id = storage_.put(axon);

    neuron->setPoint(point);
    axon->setPoint(neuron->point());

    neuron->setInit(neuron_id, brain_map_, &data_collector_);
    neuron->init(axon_id, "");

    axon->setInit(axon_id, brain_map_, &data_collector_);

    return {
        {"neuron", neuron_id},
        {"axon", axon_id}
    };
}

std::unordered_map<std::string, std::string> Controller::createOutputNeuron(BrainPoint point) {
    auto *neuron = new Neuron(&executor_, NeuronType::OUTPUT);
    auto neuron_id = storage_.put(neuron);

    auto *axon = new Axon(&executor_, neuron_id);
    auto axon_id = storage_.put(axon);

    auto *dendrite = new Dendrite(&executor_, neuron_id);
    auto dendrite_id = storage_.put(dendrite);

    neuron->setPoint(point);
    axon->setPoint(neuron->point());
    dendrite->setPoint(neuron->point());

    neuron->setInit(neuron_id, brain_map_, &data_collector_);
    neuron->init(axon_id, dendrite_id);

    dendrite->setInit(dendrite_id, brain_map_, &data_collector_);
    neuron->setInit(neuron_id, brain_map_, &data_collector_);
    axon->setInit(axon_id, brain_map_, &data_collector_);


    return {
        {"neuron", neuron_id},
        {"axon", axon_id},
        {"dendrite", dendrite_id}
    };
}
