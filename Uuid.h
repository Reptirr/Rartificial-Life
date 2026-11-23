#pragma once

#include <array>
#include <cstdint>
#include <cstddef>
#include <string>
#include <random>
#include <sstream>
#include <iomanip>

struct Uuid {
    std::array<std::uint8_t, 16> bytes{};

    bool operator==(const Uuid&) const noexcept = default;
    bool operator!=(const Uuid& other) const noexcept { return !(*this == other); }

    // Default constructor - creates nil UUID
    Uuid() noexcept = default;

    // Constructor from bytes array
    explicit Uuid(const std::array<std::uint8_t, 16>& b) noexcept : bytes(b) {}

    // Constructor from string (UUID format: xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx)
    explicit Uuid(const std::string& str) {
        parse(str);
    }

    // Check if UUID is nil (all zeros)
    bool isNil() const noexcept {
        for (auto b : bytes) {
            if (b != 0) return false;
        }
        return true;
    }

    bool empty() const noexcept {
        return isNil();
    }

    // Convert to string representation
    std::string toString() const {
        std::ostringstream oss;
        oss << std::hex << std::setfill('0')
            << std::setw(2) << static_cast<int>(bytes[0])
            << std::setw(2) << static_cast<int>(bytes[1])
            << std::setw(2) << static_cast<int>(bytes[2])
            << std::setw(2) << static_cast<int>(bytes[3])
            << '-'
            << std::setw(2) << static_cast<int>(bytes[4])
            << std::setw(2) << static_cast<int>(bytes[5])
            << '-'
            << std::setw(2) << static_cast<int>(bytes[6])
            << std::setw(2) << static_cast<int>(bytes[7])
            << '-'
            << std::setw(2) << static_cast<int>(bytes[8])
            << std::setw(2) << static_cast<int>(bytes[9])
            << '-'
            << std::setw(2) << static_cast<int>(bytes[10])
            << std::setw(2) << static_cast<int>(bytes[11])
            << std::setw(2) << static_cast<int>(bytes[12])
            << std::setw(2) << static_cast<int>(bytes[13])
            << std::setw(2) << static_cast<int>(bytes[14])
            << std::setw(2) << static_cast<int>(bytes[15]);
        return oss.str();
    }

    // Generate a random UUID v4
    static Uuid generate() {
        static thread_local std::random_device rd;
        static thread_local std::mt19937_64 gen{
            (static_cast<uint64_t>(rd()) << 32) ^ rd()
        };
        std::uniform_int_distribution<uint64_t> dist;

        uint64_t hi = dist(gen);
        uint64_t lo = dist(gen);

        hi = (hi & 0xFFFFFFFFFFFF0FFFULL) | 0x0000000000004000ULL; // version 4
        lo = (lo & 0x3FFFFFFFFFFFFFFFULL) | 0x8000000000000000ULL; // RFC 4122 variant

        Uuid uuid;
        // hi bytes (8 bytes)
        uuid.bytes[0] = static_cast<uint8_t>(hi >> 56);
        uuid.bytes[1] = static_cast<uint8_t>(hi >> 48);
        uuid.bytes[2] = static_cast<uint8_t>(hi >> 40);
        uuid.bytes[3] = static_cast<uint8_t>(hi >> 32);
        uuid.bytes[4] = static_cast<uint8_t>(hi >> 24);
        uuid.bytes[5] = static_cast<uint8_t>(hi >> 16);
        uuid.bytes[6] = static_cast<uint8_t>(hi >> 8);
        uuid.bytes[7] = static_cast<uint8_t>(hi);
        // lo bytes (8 bytes)
        uuid.bytes[8] = static_cast<uint8_t>(lo >> 56);
        uuid.bytes[9] = static_cast<uint8_t>(lo >> 48);
        uuid.bytes[10] = static_cast<uint8_t>(lo >> 40);
        uuid.bytes[11] = static_cast<uint8_t>(lo >> 32);
        uuid.bytes[12] = static_cast<uint8_t>(lo >> 24);
        uuid.bytes[13] = static_cast<uint8_t>(lo >> 16);
        uuid.bytes[14] = static_cast<uint8_t>(lo >> 8);
        uuid.bytes[15] = static_cast<uint8_t>(lo);
        return uuid;
    }

private:
    void parse(const std::string& str) {
        // Simple parser for UUID format xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
        std::string cleaned;
        cleaned.reserve(32);
        for (char c : str) {
            if (c != '-') cleaned.push_back(c);
        }
        if (cleaned.size() != 32) return; // Invalid format, leave as nil

        for (size_t i = 0; i < 16; ++i) {
            std::string byte_str = cleaned.substr(i * 2, 2);
            bytes[i] = static_cast<uint8_t>(std::stoi(byte_str, nullptr, 16));
        }
    }
};

struct UuidHash {
    std::size_t operator()(const Uuid& uuid) const noexcept {
        std::uint64_t hash = 14695981039346656037ULL;

        for (auto byte : uuid.bytes) {
            hash ^= byte;
            hash *= 1099511628211ULL;
        }

        return static_cast<std::size_t>(hash);
    }
};

// Specialize std::hash for Uuid
namespace std {
    template<>
    struct hash<Uuid> {
        std::size_t operator()(const Uuid& uuid) const noexcept {
            return UuidHash{}(uuid);
        }
    };
}