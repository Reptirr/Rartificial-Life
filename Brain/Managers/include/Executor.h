#pragma once

#include <queue>
#include <utility>
#include <Storage.h>
#include <Command.h>

/*
 * первый такт: 1. Neuron (стреляет)
 * (пропадает) второй такт: 2. Axon (передает синапсам)
 * третий такт: 3. Synaps (имеет коэфицент, обучается)
 * четвертый такт: 4. Dendrit (суммирует сигналы синапсов, отдает в нейрон)
 */

struct Command;

static inline std::string mapToString(const std::unordered_map<std::string, std::string>& map) {
    std::string res{};

    for (const auto& [key, val] : map) {
        res += key + ": " + val + "; ";
    }

    return res;
}


class Executor {
    Storage &storage_;
    std::queue<Command> dirties_{};
    std::queue<Command> next_tact_dirties{};

    unsigned int tact_i{};
    bool tact_running{};

    std::vector<PlanCommand> plan_commands_{};

    void executeCmdFabric(Command cmd);

public:
    explicit Executor(Storage &storage) : storage_(storage) {

    }

    void pushPlan(int inTact, Command cmd) {
        plan_commands_.push_back(PlanCommand{cmd, inTact});
    }

    void push(const Command &cmd) {
        if (tact_running)
            next_tact_dirties.push(cmd);
        else
            dirties_.push(cmd);
    }

    unsigned int tact();
};
