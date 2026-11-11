#include <my_assert.h>
#include <WorldUIController.h>
#include <WorldView.h>

void WorldUIController::newWorldObject(const std::string &id, WorldObjectData *data) {
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

    QObject::connect(item, &WorldObjectItem::mouseDoubleClick, view_, &WorldView::onAgentOpen);

    // !!!костыль
    item->setData(data);

    storage_.putObject(id, item);
    scene_->addItem(item);
}
