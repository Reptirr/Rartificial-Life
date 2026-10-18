#pragma once

#include <chrono>
#include <iostream>

class BrainAnalyzer {
    using Clock = std::chrono::steady_clock;

    Clock::time_point m_start;
    Clock::time_point m_lastReport = Clock::now();
    double m_tactSeconds = 0;
    int m_tacts = 0;

    int dirties_num{};
    int plan_num{};

    CommandType cmd_type{};
    Clock::time_point cmd_start;

    std::unordered_map<CommandType, double> cmd_durs_; // на 1 такт. рассчитывается время команд этого типа в текущем такте

public:
    void beginTact(int dirties_n, int plan_n) {
        m_start = Clock::now();

        dirties_num = dirties_n;
        plan_num = plan_n;
    }

    void endTact() {
        auto now = Clock::now();
        m_tactSeconds += std::chrono::duration<double>(now - m_start).count();
        ++m_tacts;

        if (now - m_lastReport >= std::chrono::seconds(1)) {
            double avgMs = m_tactSeconds / m_tacts * 1000.0;
            std::cout << "cmd handled: " << dirties_num << "; cmd planned: " << plan_num << "\n";
            std::cout << "tps: " << m_tacts
                      << ", avg tact: " << avgMs << " ms\n";
            std::cout << "cmd_durs: \n";
            for (auto [type, dur] : cmd_durs_) {
                std::cout << toString(type) << ": " << dur * 1000.0 << "ms\n";
            }
            std::cout << "\n\n";

            m_tacts = 0;
            m_tactSeconds = 0;
            m_lastReport = now;
            cmd_durs_ = std::unordered_map<CommandType, double>{};
        }
    }

    void beginCommand(CommandType type) {
        cmd_start = Clock::now();
        cmd_type = type;
    }

    void endCommand() {
        auto now = Clock::now();
        double cmd_dur = std::chrono::duration<double>(now - cmd_start).count();

        cmd_durs_.insert({cmd_type, cmd_dur});
    }
};