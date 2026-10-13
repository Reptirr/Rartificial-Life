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

        case SYNAPS_FORWARD_SIGNAL: {
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

        default: return;
    }
}

unsigned int Executor::tact() {
    tact_running = true;
    tact_i++;

    int action_i = 0;

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
