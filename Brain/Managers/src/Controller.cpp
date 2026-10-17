#include <Axon.h>
#include <Controller.h>
#include <Dendrite.h>
#include <Neuron.h>
#include <Synaps.h>

Controller::Controller(Storage &storage, Executor &executor)
    : storage_(storage), executor_(executor)
{

}

std::unordered_map<std::string, std::string> Controller::createNeuron() {
    auto *neuron = new Neuron(&executor_);
    auto neuron_id = storage_.put(neuron);

    auto *axon = new Axon(&executor_, neuron_id);
    auto axon_id = storage_.put(axon);

    auto *dendrite = new Dendrite(&executor_, neuron_id);
    auto dendrite_id = storage_.put(dendrite);

    neuron->setThisId(neuron_id);
    neuron->init(axon_id, dendrite_id);

    // random coordinates
    static std::random_device rd{};
    static std::mt19937 gen{rd()};
    static std::uniform_real_distribution dist{-1.0, 1.0};

    neuron->setPoint({ dist(gen), dist(gen) });
    axon->setPoint(neuron->point());
    dendrite->setPoint(neuron->point());

    dendrite->setThisId(dendrite_id);
    neuron->setThisId(neuron_id);
    axon->setThisId(axon_id);

    return {
        {"neuron", neuron_id},
        {"axon", axon_id},
        {"dendrite", dendrite_id}
    };

}

std::string Controller::createSynaps(const std::string &dendrite_id, const std::string &axon_id) {
    auto *synaps = new Synaps(&executor_, dendrite_id, axon_id);
    auto synaps_id = storage_.put(synaps);

    auto *axon = storage_.get<Axon>(axon_id);
    auto *dendrite = storage_.get<Dendrite>(dendrite_id);

    axon->addSynaps(synaps_id);
    dendrite->addSynaps(synaps_id);

    synaps->setThisId(synaps_id);

    return synaps_id;
}
