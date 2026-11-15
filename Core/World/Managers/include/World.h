#pragma once
#include <memory>
#include <WorldDataCollector.h>


class QColor;
class QRectF;
class QPointF;
class SimulationBridge;
class WorldExecutor;
class WorldController;
class WorldStorage;
struct AgentBodyConfig;

class World {
    std::unique_ptr<WorldStorage> storage_;
    std::unique_ptr<WorldController> controller_;
    std::unique_ptr<WorldExecutor> executor_;
    WorldDataCollector data_collector_;

    double dt_;

public:
    explicit World(double dt);
    void init(SimulationBridge *controller);

    void tick();

    std::string addAgentBody(const AgentBodyConfig &conf) const;
    std::string addObstacle(QPointF pos, QRectF rect, QColor color) const;
    std::string addFood(QPointF pos) const;

    const WorldStorage *getStorage() const;

    // data collector
    void setCreateConsumer(const StringConsumer &cons);
    void setUpdateConsumer(const StringConsumer &cons);
    void setRemoveConsumer(const StringConsumer &cons);
};
