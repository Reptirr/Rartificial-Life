#pragma once
#include <queue>
#include <string>
#include <unordered_map>
#include <WorldUIController.h>
#include <WorldUIDatas.h>
#include <WorldUIStorage.h>

struct WorldUIToCreate {
    std::string id;
    WorldObjectData data;
};

class WorldUIBatcher {
    WorldUIStorage &storage_;
    WorldUIController &controller_;

    std::queue<WorldUIToCreate> to_create_;
    std::unordered_map<std::string, WorldObjectData> to_update_;

    std::mutex mtx_;

public:
    explicit WorldUIBatcher(WorldUIStorage &storage, WorldUIController &controller)
        : storage_(storage), controller_(controller) {
    }

    void onCreate(const std::string &id, const WorldObjectData &data) {
        std::lock_guard lock(mtx_);
        to_create_.push(WorldUIToCreate{id, data});
    }
    void onUpdate(const std::string &id, WorldObjectData data) {
        std::lock_guard lock(mtx_);
        to_update_.insert_or_assign(id, data);
    }

    void applyBatch() {
        std::queue<WorldUIToCreate> to_create;
        std::unordered_map<std::string, WorldObjectData> to_update;

        {
            std::lock_guard lock(mtx_);

            to_create.swap(to_create_);
            to_update.swap(to_update_);
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
    }
};
