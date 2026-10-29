#pragma once

#include <SenseGetter.h>
#include <WorldStorage.h>

class WorldController {
    WorldStorage &storage_;
    SenseGetter getter_;

public:
    explicit WorldController(WorldStorage &storage)
        : storage_(storage), getter_(storage) {

    }

    // with storage work
    std::string addAgentBody(QPointF pos) const;

    std::string addObstacle(QPointF) const;
};
