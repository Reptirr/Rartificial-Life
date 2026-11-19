#include <MainExperiment.h>
#include <SimulationController.h>
#include <World.h>

void MainExperiment::preFirstTick(TickContext &ctx) {
    // создаем начальных агентов
    for (int i = 0; i < 1; i++) {
        auto agent_def = AgentDef{
            .agent_body_config = {
                .pos = {Rng::randFloat(-100, 100), Rng::randFloat(-100, 100)},
                .rays = 64,
                .fov = std::numbers::pi,

                .move_dirs = 40
            },
            .agent_config = {
                .genome = GenomeRandomizer::randomizing(),
                .num_of_common_neurons = 300
            }
        };

        ctx.sim_controller->createAgent(agent_def);
    }


    // создаем начальную еду для популяции
    for (int i = 0; i < 10000; i++) {
        ctx.world->addFood({Rng::randReal(-100, 100), Rng::randReal(-100, 100)});
    }
}

void MainExperiment::onTick(TickContext &ctx) {
    /*
     * создание еды
     *
     * делаем механику клеток, всего 9 клеток в 100x100 площади, заполняем постепенно, раз в x тактов создаем еду
     * заполняем 1 клетку, потом начинаем заполнять только другую
     *
     * 0 1 2
     * 3 4 5
     * 6 7 8
     */

    if (ctx.tick_i % 10 == 0 && tact_per_food < 500) tact_per_food+=1;
    if (ctx.tick_i % tact_per_food == 0) {
        if (food_i >= food_for_cell) {
            now_cell = Rng::randInt(0, 8);
            food_i = 0;
        }

        // coors
        const auto cell_x = now_cell % 3;
        const auto cell_y = now_cell / 3;

        const double x_min = cell_x * cell_size;
        const double x_max = (cell_x + 1) * cell_size;
        const double y_min = cell_y * cell_size;
        const double y_max = (cell_y + 1) * cell_size;

        // spawn
        ctx.world->addFood({Rng::randReal(x_min, x_max), Rng::randReal(y_min, y_max)});
        food_i++;
    }
}
