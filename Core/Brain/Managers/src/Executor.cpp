#include <Axon.h>
#include <Controller.h>
#include <Dendrite.h>
#include <Executor.h>
#include <iostream>
#include <Neuron.h>
#include <Storage.h>
#include <Synaps.h>
#include <Uuid.h>

void Executor::executeCmdFabric(const Command &cmd) {
    switch (cmd.type) {
        case NONE: {
            return;
        }

        case AXON_GET_SPIKE: {
            const Uuid &axon_id = cmd.data.at("axon").uuid_var;
            auto *axon = storage_.get<Axon>(axon_id);
            my_assert(axon != nullptr);

            axon->putSpike();
            return;
        }

        case SYNAPS_GET_SPIKE: {
            const Uuid &synaps_id = cmd.data.at("synaps").uuid_var;
            auto *synaps = storage_.get<Synaps>(synaps_id);
            my_assert(synaps != nullptr);

            synaps->handleSpike(tact_i);
            return;
        }

        case DENDRIT_GET_SIGNAL: {
            const Uuid &dendrite_id = cmd.data.at("dendrite").uuid_var;
            const std::string &signal = cmd.data.at("signal").string_var;

            auto *dendrite = storage_.get<Dendrite>(dendrite_id);
            my_assert(dendrite != nullptr);

            dendrite->addSignal(signal);
            return;
        }

        case NEURON_GET_SIGNAL: {
            const Uuid &neuron_id = cmd.data.at("neuron").uuid_var;
            const std::string &signal = cmd.data.at("signal").string_var;

            auto *neuron = storage_.get<Neuron>(neuron_id);
            my_assert(neuron != nullptr);

            neuron->addSignal(signal);
            return;
        }

        case HANDLE_CA_IN_DENDRITE: {
            const Uuid &dendrite_id = cmd.data.at("dendrite").uuid_var;
            auto *dendrite = storage_.get<Dendrite>(dendrite_id);
            my_assert(dendrite != nullptr);

            dendrite->transmitCu();
            return;
        }

        case HANDLE_CA_IN_SYNAPS: {
            const Uuid &synaps_id = cmd.data.at("synaps").uuid_var;
            auto *synaps = storage_.get<Synaps>(synaps_id);
            my_assert(synaps != nullptr);

            synaps->handleCa(tact_i);
            return;
        }

        case RANDOM_NEURON_SPIKE: {
            const Uuid &neuron_id = cmd.data.at("neuron").uuid_var;
            auto *neuron = storage_.get<Neuron>(neuron_id);
            my_assert(neuron != nullptr);

            neuron->onRandomSpike();
            return;
        }

        case AXON_GREW: {
            const Uuid &axon_id = cmd.data.at("axon").uuid_var;
            auto *axon = storage_.get<Axon>(axon_id);
            my_assert(axon != nullptr);

            axon->handleGrow();
            return;
        }

        case CREATE_SYNAPS: {
            const int node_i = std::stoi(cmd.data.at("current_node_index").string_var);

            const Uuid &axon_id = cmd.data.at("axon").uuid_var;
            const Uuid &dendrite_id = cmd.data.at("dendrite").uuid_var;

            my_assert(storage_.get<Axon>(axon_id) != nullptr);
            my_assert(storage_.get<Dendrite>(dendrite_id) != nullptr);

            controller_.createSynaps(dendrite_id, axon_id, node_i);
            return;
        }

        case NEURON_REFRACTOR_RESET: {
            const Uuid &neuron_id = cmd.data.at("neuron").uuid_var;
            auto *neuron = storage_.get<Neuron>(neuron_id);
            my_assert(neuron != nullptr);

            neuron->refractorReset();
            return;
        }

        default:
            my_assert(false);
    }
}

unsigned int Executor::tact() {
    analyzer_.beginTact(dirties_.size(), plan_commands_.size());

    tact_running = true;
    tact_i++;

    int action_i = 0;

    // проверяем запланированные команды
    while (!plan_commands_.empty() && plan_commands_.top().execute_at <= tact_i) {
        dirties_.push(plan_commands_.top().cmd);
        plan_commands_.pop();
    }

    while (!dirties_.empty()) {
        analyzer_.beginCommand(dirties_.front().type);
        executeCmdFabric(dirties_.front());
        analyzer_.endCommand();

        dirties_.pop();
        action_i++;
    }

    tact_running = false;
    std::swap(dirties_, next_tact_dirties);

    analyzer_.endTact();
    return action_i;
}