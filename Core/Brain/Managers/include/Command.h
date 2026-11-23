#pragma once
#include <magic_enum.hpp>
#include <string>
#include <unordered_map>

enum CommandType : int {
    NONE,

    // BASE
    AXON_GET_SPIKE,
    SYNAPS_GET_SPIKE,
    DENDRIT_GET_SIGNAL,
    NEURON_GET_SIGNAL,

    // EDU
    HANDLE_CA_IN_DENDRITE,
    HANDLE_CA_IN_SYNAPS,

    // GROWING
    AXON_GREW,
    CREATE_SYNAPS,

    // OTHER
    RANDOM_NEURON_SPIKE,
    NEURON_REFRACTOR_RESET
};

inline std::string_view toString(CommandType type) {
    return magic_enum::enum_name(type);
}

enum VarType {
    STRING,
    UUID
};
struct CommandVar {
    VarType type;
    std::string string_var{};
    Uuid uuid_var{};

    CommandVar(const std::string &s) : type(STRING), string_var(s) {}
    CommandVar(std::string &&s)      : type(STRING), string_var(std::move(s)) {}
    CommandVar(const char *s)        : type(STRING), string_var(s) {}
    CommandVar(const Uuid &id)       : type(UUID),   uuid_var(id) {}
};

struct Command {
    CommandType type{};
    std::unordered_map<std::string, CommandVar> data{};
};

struct PlanCommand {
    Command cmd{};
    unsigned int execute_at;
};

struct PlanCommandCompare {
    bool operator()(const PlanCommand& a,
                    const PlanCommand& b) const {
        return a.execute_at > b.execute_at;
    }
};