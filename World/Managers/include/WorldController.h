#pragma once

#include <MoveController.h>
#include <SenseGetter.h>
#include <WorldStorage.h>

class WorldController {
    WorldStorage &storage_;
    SenseGetter getter_;
    MoveController move_;

public:
    explicit WorldController(WorldStorage &storage)
        : storage_(storage), getter_(storage), move_(storage) {

    }

    // with storage work
    std::string addAgentBody(QPointF pos, int rays) const;

    std::string addObstacle(QPointF) const;
};
