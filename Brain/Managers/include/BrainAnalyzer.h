#pragma once

#include <algorithm>
#include <chrono>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

struct CmdInfo {
    double dur{};   // суммарное время команд этого типа за период отчёта (секунды)
    int calls{};    // сколько раз выполнялась
};

class BrainAnalyzer {
    using Clock = std::chrono::steady_clock;

    Clock::time_point m_start;
    Clock::time_point m_lastReport = Clock::now();
    double m_tactSeconds = 0;
    int m_tacts = 0;

    // суммы за период отчёта, чтобы печатать среднее на такт, а не случайный срез
    long long m_handledSum = 0;
    long long m_plannedSum = 0;

    CommandType cmd_type{};
    Clock::time_point cmd_start;

    std::unordered_map<CommandType, CmdInfo> cmd_durs_;   // накопление за период отчёта

public:
    void beginTact(int dirties_n, int plan_n) {
        m_start = Clock::now();
        m_handledSum += dirties_n;
        m_plannedSum += plan_n;
    }

    void endTact() {
#ifndef BRAIN_DEBUG
      return;
#endif

        const auto now = Clock::now();
        m_tactSeconds += std::chrono::duration<double>(now - m_start).count();
        ++m_tacts;

        if (now - m_lastReport < std::chrono::seconds(1)) return;

        const double avgMs = m_tactSeconds / m_tacts * 1000.0;
        std::cout << "tps: " << m_tacts
                  << ", avg tact: " << avgMs << " ms"
                  << ", handled/tact: " << double(m_handledSum) / m_tacts
                  << ", planned/tact: " << double(m_plannedSum) / m_tacts << "\n";

        // сортировка по суммарному времени: сверху главное узкое место
        std::vector<std::pair<CommandType, CmdInfo>> v(cmd_durs_.begin(), cmd_durs_.end());
        std::sort(v.begin(), v.end(),
                  [](const auto& a, const auto& b) { return a.second.dur > b.second.dur; });

        // ширина колонки с названием = самое длинное имя
        size_t nameW = 4;   // длина слова "name"
        for (const auto& [type, info] : v)
            nameW = std::max(nameW, std::string(toString(type)).size());

        std::cout << "cmd_durs (per tact):\n";
        std::cout << "  " << std::left << std::setw(nameW) << "name"
                  << std::right
                  << std::setw(10) << "calls"
                  << std::setw(14) << "avg, ms"
                  << std::setw(14) << "total, ms" << "\n";

        std::cout << std::fixed << std::setprecision(4);
        for (const auto& [type, info] : v) {
            std::cout << "  " << std::left << std::setw(nameW) << toString(type)
                      << std::right
                      << std::setw(10) << double(info.calls) / m_tacts
                      << std::setw(14) << info.dur / info.calls * 1000.0
                      << std::setw(14) << info.dur / m_tacts * 1000.0 << "\n";
        }
        std::cout << std::defaultfloat << "\n";

        m_tacts = 0;
        m_tactSeconds = 0;
        m_handledSum = 0;
        m_plannedSum = 0;
        m_lastReport = now;
        cmd_durs_.clear();
    }

    void beginCommand(CommandType type) {
        cmd_type = type;
        cmd_start = Clock::now();
    }

    void endCommand() {
        const double d = std::chrono::duration<double>(Clock::now() - cmd_start).count();
        auto& info = cmd_durs_[cmd_type];   // создаст запись, если её ещё нет
        info.dur += d;
        ++info.calls;
    }
};