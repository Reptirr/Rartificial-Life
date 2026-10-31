#pragma once
#include <SceneUtils.h>
#include <WorldUIStorage.h>

class WorldUIController {
    WorldUIStorage &storage_;
    QGraphicsScene *scene_{};

public:
    WorldUIController(WorldUIStorage &storage, QGraphicsScene *scene)
        : storage_(storage),
          scene_(scene) {
    }

    void newWorldObject(const std::string &id, const WorldObjectData data) const {
        auto item = new WorldObjectItem(data);
        storage_.putObject(id, item);
        scene_->addItem(item);
    }

    void updateWorldObject(const std::string &id, const WorldObjectData data) const {
        auto item = storage_.getObject(id);
        item->setData(data);
        item->UIUpdate();
    }
};
