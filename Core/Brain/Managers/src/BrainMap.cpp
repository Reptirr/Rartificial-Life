#include <BrainMap.h>
#include <Dendrite.h>

#include <algorithm>
#include <cmath>
#include <stdexcept>

BrainMap::BrainMap(Storage* storage, double cell_size)
    : storage_(storage), cell_size_(cell_size) {

    if (!(cell_size > 0.0) || !std::isfinite(cell_size))
        throw std::invalid_argument("Invalid cell size");

    rebuildDendriteIndex();
}

BrainMap::Cell BrainMap::cellOf(BrainPoint p) const {
    return {
        static_cast<std::int64_t>(std::floor(p.x / cell_size_)),
        static_cast<std::int64_t>(std::floor(p.y / cell_size_))
    };
}

void BrainMap::rebuildDendriteIndex() {
    cells_.clear();
    cell_by_id_.clear();

    if (!storage_)
        return;

    const auto& dendrites = storage_->dendrites();

    cells_.reserve(dendrites.size());
    cell_by_id_.reserve(dendrites.size());

    for (const auto& id : dendrites) {
        auto* dendrite = storage_->get<Dendrite>(id);

        if (!dendrite)
            continue;

        const auto cell = cellOf(dendrite->point());

        cells_[cell].push_back(id);
        cell_by_id_.emplace(id, cell);
    }
}

void BrainMap::removeFromBucket(
    const Cell& cell,
    const Uuid& id
) {
    auto it = cells_.find(cell);

    if (it == cells_.end())
        return;

    auto& ids = it->second;

    ids.erase(
        std::remove(ids.begin(), ids.end(), id),
        ids.end()
    );

    if (ids.empty())
        cells_.erase(it);
}

void BrainMap::updateDendrite(const Uuid& id) {
    if (!storage_)
        return;

    auto* dendrite = storage_->get<Dendrite>(id);

    if (!dendrite) {
        eraseDendrite(id);
        return;
    }

    const auto new_cell = cellOf(dendrite->point());
    auto it = cell_by_id_.find(id);

    if (it != cell_by_id_.end()) {
        if (it->second == new_cell)
            return;

        removeFromBucket(it->second, id);
        it->second = new_cell;
    } else {
        cell_by_id_.emplace(id, new_cell);
    }

    cells_[new_cell].push_back(id);
}

void BrainMap::eraseDendrite(const Uuid& id) {
    auto it = cell_by_id_.find(id);

    if (it == cell_by_id_.end())
        return;

    removeFromBucket(it->second, id);
    cell_by_id_.erase(it);
}

std::vector<Uuid> BrainMap::searchDendrite(
    BrainPoint from,
    double r
) const {
    std::vector<Uuid> res;

    if (!storage_ || !(r >= 0.0) || !std::isfinite(r))
        return res;

    const double x_min = from.x - r;
    const double x_max = from.x + r;
    const double y_min = from.y - r;
    const double y_max = from.y + r;

    const Cell min_cell = cellOf({x_min, y_min});
    const Cell max_cell = cellOf({x_max, y_max});

    for (auto x = min_cell.x; x <= max_cell.x; ++x) {
        for (auto y = min_cell.y; y <= max_cell.y; ++y) {
            auto it = cells_.find({x, y});

            if (it == cells_.end())
                continue;

            for (const auto& id : it->second) {
                auto* dendrite = storage_->get<Dendrite>(id);

                if (!dendrite)
                    continue;

                const auto p = dendrite->point();

                if (x_min < p.x && p.x < x_max &&
                    y_min < p.y && p.y < y_max) {
                    res.push_back(id);
                }
            }
        }
    }

    return res;
}