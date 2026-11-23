#pragma once
#include <queue>
#include <unordered_map>
#include <Uuid.h>
#include <WorldUIController.h>
#include <WorldUIDatas.h>
#include <WorldUIMapper.h>
#include <WorldUIStorage.h>

struct WorldUIToCreate {
    Uuid id;
    WorldObjectData *data;
};

class WorldUIBatcher {
    WorldUIStorage &storage_;
    WorldUIController &controller_;

    std::queue<WorldUIToCreate> to_create_;
    std::unordered_map<Uuid, WorldObjectData *> to_update_;
    std::unordered_set<Uuid> to_remove_;

    std::mutex mtx_;

public:
    explicit WorldUIBatcher(WorldUIStorage &storage, WorldUIController &controller)
        : storage_(storage), controller_(controller) {
    }

    void onCreate(const Uuid &id, WorldObjectData *data) {
        std::lock_guard lock(mtx_);
        to_create_.push(WorldUIToCreate{id, data});
    }
    void onUpdate(const Uuid &id, WorldObjectData *data) {
        std::lock_guard lock(mtx_);
        to_update_.insert_or_assign(id, data);
    }
    void onRemove(const Uuid &id) {
        std::lock_guard lock(mtx_);
        to_remove_.insert(id);
    }

    void applyBatch() {
        std::queue<WorldUIToCreate> to_create;
        std::unordered_map<Uuid, WorldObjectData *> to_update;
        std::unordered_set<Uuid> to_remove;

        {
            std::lock_guard lock(mtx_);

            to_create.swap(to_create_);
            to_update.swap(to_update_);
            to_remove.swap(to_remove_);
        }

        // creates
        while (!to_create.empty()) {
            auto data = to_create.front();
            to_create.pop();
            controller_.newWorldObject(data.id, data.data);
        }

        // updates
        for (auto [id, data] : to_update) {
            controller_.updateWorldObject(id, data);
        }

        // removes
        for (auto id : to_remove) {
            controller_.removeWorldObject(id);
        }
    }
};
