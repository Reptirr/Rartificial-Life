#pragma once
#include <queue>
#include <string>
#include <unordered_map>

class WorldController;
class WorldStorage;

enum class WorldCommandType {
    NONE,

    AGENT_BODY_EAT_FOOD,
    AGENT_BODY_MOVE,
    AGENT_BODY_ROTATE
};

struct WorldCommand {
    WorldCommandType type{};
    std::unordered_map<std::string, std::string> data;
};

class WorldExecutor {
    WorldStorage *storage_{};
    WorldController *controller_{};

    mutable std::queue<WorldCommand> commands_;
    void handleCmdFabric(const WorldCommand &cmd);

public:
    void init(WorldStorage *storage, WorldController *controller) {
        storage_ = storage;
        controller_ = controller;
    }

    void push(const WorldCommand &cmd) const {
        commands_.push(cmd);
    }

    void apply() {
        std::queue<WorldCommand> commands;
        commands.swap(commands_);

        while (!commands.empty()) {
            handleCmdFabric(commands.front());
            commands.pop();
        }
    }
};