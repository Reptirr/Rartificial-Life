#pragma once

#include <BrainComponent.h>
#include <Storage.h>

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

class BrainMap {
    struct Cell {
        std::int64_t x;
        std::int64_t y;

        bool operator==(const Cell& other) const noexcept {
            return x == other.x && y == other.y;
        }
    };

    struct CellHash {
        std::size_t operator()(const Cell& cell) const noexcept {
            const auto h1 = std::hash<std::int64_t>{}(cell.x);
            const auto h2 = std::hash<std::int64_t>{}(cell.y);

            return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
        }
    };

    Storage* storage_;
    double cell_size_;

    // ячейка -> идентификаторы дендритов
    std::unordered_map<Cell, std::vector<std::string>, CellHash> cells_;

    // айди дендрита -> его ячейка
    std::unordered_map<std::string, Cell> cell_by_id_;

    Cell cellOf(BrainPoint p) const;

    void removeFromBucket(const Cell& cell, const std::string& id);

public:
    explicit BrainMap(Storage* storage, double cell_size = 1.0);

    void rebuildDendriteIndex();
    void updateDendrite(const std::string& id);
    void eraseDendrite(const std::string& id);

    std::vector<std::string> searchDendrite(BrainPoint from, double r) const;
};