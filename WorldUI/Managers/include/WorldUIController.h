#pragma once
#include <AgentBodyItem.h>
#include <EntityKind.h>
#include <FoodItem.h>
#include <WorldUIStorage.h>

class WorldView;

class WorldUIController {
    WorldUIStorage &storage_;
    WorldView *view_{};
    QGraphicsScene *scene_{};

public:
    WorldUIController(WorldUIStorage &storage, QGraphicsScene *scene, WorldView *view)
        : storage_(storage),
          view_(view), scene_(scene) {
    }

    void newWorldObject(const std::string &id, WorldObjectData *data);

    void updateWorldObject(const std::string &id, WorldObjectData *data) const {
        auto item = storage_.getObject(id);
        item->setData(data);
        item->UIUpdate();
    }

    void removeWorldObject(const std::string &id) const {
        const auto item = storage_.getObject(id);
        storage_.removeObject(id);
        delete item; // remove from scene
    }
};
