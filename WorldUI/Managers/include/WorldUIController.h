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
          scene_(scene), view_(view) {
    }

    void newWorldObject(const std::string &id, WorldObjectData *data);

    void updateWorldObject(const std::string &id, WorldObjectData *data) const {
        auto item = storage_.getObject(id);
        item->setData(data);
        item->UIUpdate();
    }
};
