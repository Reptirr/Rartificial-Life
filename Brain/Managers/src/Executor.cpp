#include <Axon.h>
#include <Dendrite.h>
#include <Executor.h>
#include <iostream>
#include <Neuron.h>
#include <Synaps.h>

void Executor::executeCmdFabric(Command cmd) {
    switch (cmd.type) {
        case NONE: {
            std::cout << "get NONE command. data: " << mapToString(cmd.data) << std::endl;
            return;
        }

        case NEURON_SPIKE: {
            std::cout << "get NEURON_SPIKE command. data: " << mapToString(cmd.data) << std::endl;

            std::string axon_id = cmd.data.at("axon");
            auto *axon = storage_.get<Axon>(axon_id);

            axon->putSpike();
            return;
        }

        case SYNAPS_GET_SPIKE: {
            std::cout << "get SYNAPS_FORWARD_SIGNAL command. data: " << mapToString(cmd.data) << std::endl;

            std::string synaps_id = cmd.data.at("synaps");
            auto *synaps = storage_.get<Synaps>(synaps_id);

            synaps->handleSpike();
            return;
        }

        case DENDRIT_GET_SIGNAL:  {
            std::cout << "get DENDRIT_GET_SIGNAL command. data: " << mapToString(cmd.data) << std::endl;

            std::string dendrite_id = cmd.data.at("dendrite");
            std::string signal = cmd.data.at("signal");

            auto *dendrite = storage_.get<Dendrite>(dendrite_id);

            dendrite->addSignal(signal);
            return;
        }

        case NEURON_GET_SIGNAL: {
            std::cout << "get NEURON_GET_SIGNAL command. data: " << mapToString(cmd.data) << std::endl;

            std::string neuron_id = cmd.data.at("neuron");
            std::string signal_s = cmd.data.at("signal");

            auto *neuron = storage_.get<Neuron>(neuron_id);

            neuron->addSignal(signal_s);

            return;
        }

        case HANDLE_CA_IN_DENDRITE: {
            std::cout << "get HANDLE_CA_IN_DENDRITE command. data: " << mapToString(cmd.data) << std::endl;

            std::string dendrite_id = cmd.data.at("dendrite");
            auto *dendrite = storage_.get<Dendrite>(dendrite_id);

            dendrite->transmitCu();

            return;
        }

        case HANDLE_CA_IN_SYNAPS: {
            std::cout << "get PRODUCE_CA_IN_SYNAPS command. data: " << mapToString(cmd.data) << std::endl;

            auto synaps_id = cmd.data.at("synaps");
            auto *synaps = storage_.get<Synaps>(synaps_id);

            synaps->handleCa();

            return;
        }

        case REDUCE_VALS_IN_SYNAPS: {
            std::cout << "get REDUCE_VALS_IN_SYNAPS command. data: " << mapToString(cmd.data) << std::endl;

            auto synaps_id = cmd.data.at("synaps");
            auto *synaps = storage_.get<Synaps>(synaps_id);

            synaps->reduce();

            return;
        }

        case RANDOM_NEURON_SPIKE: {
            std::cout << "get RANDOM_NEURON_SPIKE command. data: " << mapToString(cmd.data) << std::endl;

            auto neuron_id = cmd.data.at("neuron");
            auto *neuron = storage_.get<Neuron>(neuron_id);

            neuron->onRandomSpike();

            return;
        }

        case AXON_GREW: {
            std::cout << "get AXON_GREW command. data: " << mapToString(cmd.data) << std::endl;

            auto axon_id = cmd.data.at("axon");
            auto *axon = storage_.get<Axon>(axon_id);

            axon->handleGrow();

            return;
        }

        case CREATE_SYNAPS: {
            std::cout << "get CREATE_SYNAPS. data: " << mapToString(cmd.data) << std::endl;

            auto node_i = std::stoi(cmd.data.at("current_node_index"));

            auto axon_id = cmd.data.at("axon");
            auto *axon = storage_.get<Axon>(axon_id);

            auto dendrite_id = cmd.data.at("dendrite");
            auto *dendrite = storage_.get<Dendrite>(dendrite_id);

            auto synaps_id = controller_.createSynaps(dendrite_id, axon_id, node_i);

            return;
        }

        default: my_assert(false);
    }
}

unsigned int Executor::tact() {
    tact_running = true;
    tact_i++;

    int action_i = 0;

    // чекаем планы
    for (auto it = plan_commands_.begin(); it != plan_commands_.end(); ) {
        if (it->inTact <= 0) {
            dirties_.push(it->cmd);
            it = plan_commands_.erase(it);  // erase возвращает следующий элемент
        } else {
            it->inTact--;
            ++it;
        }
    }

    while (!dirties_.empty()) {
        std::cout << "[" << tact_i << "][" << action_i << "] ";
        executeCmdFabric(dirties_.front());
        dirties_.pop();
        action_i++;
    }

    tact_running = false;
    dirties_ = next_tact_dirties;
    next_tact_dirties = std::queue<Command>();

    return action_i;
}
