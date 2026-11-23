#pragma once
#include <string>
#include <unordered_map>
#include <WorldObjectItem.h>
#include <Uuid.h>

class WorldUIStorage {
    std::unordered_map<Uuid, WorldObjectItem *, UuidHash> objects_;
    std::mutex mtx_;
public:
    void putObject(const Uuid &id, WorldObjectItem *item) {
        std::lock_guard lock(mtx_);
        objects_.insert({id, item});
    }

    WorldObjectItem *getObject(const Uuid &id) {
        std::lock_guard lock(mtx_);
        return objects_.at(id);
    }

    void removeObject(const Uuid &id) {
        std::lock_guard lock(mtx_);
        objects_.erase(id);
    }
};
