#pragma once

#include <random>
#include <string>
#include <cstdint>
#include <cstdio>

static inline std::string generateUuid() {
    static thread_local std::random_device rd;
    static thread_local std::mt19937_64 gen{
        (static_cast<uint64_t>(rd()) << 32) ^ rd()
    };
    std::uniform_int_distribution<uint64_t> dist;

    uint64_t hi = dist(gen);
    uint64_t lo = dist(gen);

    hi = (hi & 0xFFFFFFFFFFFF0FFFULL) | 0x0000000000004000ULL; // версия 4
    lo = (lo & 0x3FFFFFFFFFFFFFFFULL) | 0x8000000000000000ULL; // вариант RFC 4122

    char buf[37];
    std::snprintf(buf, sizeof buf, "%08x-%04x-%04x-%04x-%012llx",
        (unsigned)(hi >> 32),
        (unsigned)((hi >> 16) & 0xFFFF),
        (unsigned)(hi & 0xFFFF),
        (unsigned)(lo >> 48),
        (unsigned long long)(lo & 0xFFFFFFFFFFFFULL));
    return buf;
}