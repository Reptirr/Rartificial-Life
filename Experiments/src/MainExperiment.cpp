#include <GenomeRandomizer.h>
#include <MainExperiment.h>
#include <SimulationController.h>
#include <World.h>
#include <WorldStorage.h>

void MainExperiment::preFirstTick(TickContext &ctx) {
    // создаем начальных агентов
    for (int i = 0; i < init_agents; i++) {
        auto agent_def = AgentDef{
            .agent_body_config = {
                .pos = {Rng::randFloat(-50, 50), Rng::randFloat(-10, 10)},
                .rays = 64,
                .fov = std::numbers::pi/2, // 90

                .move_dirs = 40
            },
            .agent_config = {
                .genome = GenomeRandomizer::randomizing(),
            }
        };

        ctx.sim_controller->createAgent(agent_def);
    }


    // создаем начальную еду для популяции
    for (int i = 0; i < init_food; i++) {
        ctx.world->addFood({Rng::randReal(-50, 50), Rng::randReal(-50, 50)});
    }

    // создаём границы карты

    // сверху
    ctx.world->addObstacle({0, 50}, {-50, 0, 100, 1}, QColorConstants::Cyan);

    // снизу
    ctx.world->addObstacle({0, -50}, {-50, 0, 100, 1}, QColorConstants::Cyan);

    // слева
    ctx.world->addObstacle({-50, 0}, {0, -50, 1, 100}, QColorConstants::Cyan);

    // справа
    ctx.world->addObstacle({50, 0}, {0, -50, 1, 100}, QColorConstants::Cyan);
}

void MainExperiment::onTick(TickContext &ctx) {
    if (ctx.world->getStorage()->getFood().size() < min_food) {
        ctx.world->addFood({Rng::randReal(-50, 50), Rng::randReal(-10, 10)});
    }

    if (ctx.sim_storage->agents().empty()) {
        std::cout << numbers << "ll" << std::endl;
        numbers++;
        preFirstTick(ctx);
    }
}
