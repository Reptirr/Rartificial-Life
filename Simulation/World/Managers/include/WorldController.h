#pragma once

#include <MoveController.h>
#include <SenseGetter.h>
#include <WorldDataCollector.h>
#include <WorldStorage.h>

class QRectF;

class WorldController {
    WorldStorage &storage_;
    SenseGetter getter_;
    MoveController move_;
    WorldDataCollector &collector_;


public:
    explicit WorldController(WorldStorage &storage, WorldDataCollector &collector)
        : storage_(storage), getter_(storage), move_(storage), collector_(collector) {

    }

    // with storage work
    std::string addAgentBody(QPointF pos, int rays, double fov) const;

    std::string addObstacle(QPointF, QRectF rect, QColor color) const;
};
