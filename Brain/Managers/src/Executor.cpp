#include <Axon.h>
#include <Dendrite.h>
#include <Executor.h>
#include <iostream>
#include <Neuron.h>
#include <Synaps.h>

void Executor::executeCmdFabric(Command cmd) {
    switch (cmd.type) {
        case NONE: {
            std::cout << "get NONE command. data: " << mapToString(cmd.data) << "\n";
            return;
        }

        case NEURON_SPIKE: {
            std::cout << "get NEURON_SPIKE command. data: " << mapToString(cmd.data) << "\n";

            std::string axon_id = cmd.data.at("axon");
            auto *axon = storage_.get<Axon>(axon_id);

            axon->putSpike();
            return;
        }

        case SYNAPS_FORWARD_SIGNAL: {
            std::cout << "get SYNAPS_FORWARD_SIGNAL command. data: " << mapToString(cmd.data) << "\n";

            std::string synaps_id = cmd.data.at("synaps");
            auto *synaps = storage_.get<Synaps>(synaps_id);

            synaps->getSpike();
            return;
        }

        case DENDRIT_GET_SIGNAL:  {
            std::cout << "get DENDRIT_GET_SIGNAL command. data: " << mapToString(cmd.data) << "\n";

            std::string dendrite_id = cmd.data.at("dendrite");
            std::string signal = cmd.data.at("signal");

            auto *dendrite = storage_.get<Dendrite>(dendrite_id);

            dendrite->getSignal(signal);
            return;
        }

        case NEURON_GET_SIGNAL: {
            std::cout << "get NEURON_GET_SIGNAL command. data: " << mapToString(cmd.data) << "\n";

            std::string neuron_id = cmd.data.at("neuron");
            std::string signal_s = cmd.data.at("signal");

            auto *neuron = storage_.get<Neuron>(neuron_id);

            neuron->getSignal(signal_s);

            return;
        }

        default: return;
    }
}

void Executor::tact() {
    std::cout << "handle tact " << ++tact_i << "\n";

    tact_running = true;

    auto dirties_copy = dirties_;

    while (!dirties_copy.empty()) {
        executeCmdFabric(dirties_copy.front());
        dirties_copy.pop();
    }

    tact_running = false;
    dirties_ = next_tact_dirties;
    next_tact_dirties = std::queue<Command>();
}
