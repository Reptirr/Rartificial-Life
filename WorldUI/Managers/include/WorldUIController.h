#pragma once
#include <AgentBodyItem.h>
#include <FoodItem.h>
#include <WorldUIStorage.h>

class WorldUIController {
    WorldUIStorage &storage_;
    QGraphicsScene *scene_{};

public:
    WorldUIController(WorldUIStorage &storage, QGraphicsScene *scene)
        : storage_(storage),
          scene_(scene) {
    }

    void newWorldObject(const std::string &id, WorldObjectData *data) const {
        WorldObjectItem *item{};

        switch (data->entity_kind) {
            case EntityKind::NONE:
                item = new WorldObjectItem(data);
                break;
            case EntityKind::AGENT_BODY:
                item = new AgentBodyItem(data);
                break;
            case EntityKind::FOOD:
                item = new FoodItem(data);
                break;

            default: my_assert(false);
        }

        // !!!костыль
        item->setData(data);

        storage_.putObject(id, item);
        scene_->addItem(item);
    }

    void updateWorldObject(const std::string &id, WorldObjectData *data) const {
        auto item = storage_.getObject(id);
        item->setData(data);
        item->UIUpdate();
    }
};
