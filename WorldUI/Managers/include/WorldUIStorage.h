#pragma once
#include <string>
#include <unordered_map>
#include <WorldObjectItem.h>

class WorldUIStorage {
    std::unordered_map<std::string, WorldObjectItem *> objects_;
    std::mutex mtx_;
public:
    void putObject(std::string id, WorldObjectItem *item) {
        std::lock_guard lock(mtx_);
        objects_.insert({id, item});
    }

    WorldObjectItem *getObject(const std::string &id) {
        std::lock_guard lock(mtx_);
        return objects_.at(id);
    }
};
