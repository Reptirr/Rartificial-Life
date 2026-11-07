#include <AgentBody.h>
#include <WorldStorage.h>

WorldEntity *WorldStorage::getEntity(const std::string &id) {
    auto *obj = getObject(id);

    switch (obj->getEntityKind()) {
        case EntityKind::NONE:
            return nullptr;
        case EntityKind::AGENT_BODY:
            return getAgentBody(id);
        case EntityKind::FOOD:
            return getFood(id);
    }

    return nullptr;
}
