#pragma once
#include <atomic>
#include <unordered_set>
#include <Uuid.h>

/*
 * Виды:
 * 1. съели еду
 * 2. агент появился
 * 3. агент умер
 */

struct FoodEatInfo {
    Uuid food_id;
    Uuid agent_id;
    int tick_i{};
};

struct StatSnapshot {
    std::vector<FoodEatInfo> food_eat_infos;
};

class Statistics {
    StatSnapshot snapshot_;
    std::mutex mtx_;

public:
    StatSnapshot snapshot() {
        auto snap = snapshot_;
        snapshot_ = {};
        return snap;
    }

    void onFoodEat(const FoodEatInfo &info) {
        snapshot_.food_eat_infos.push_back(info);
    }
};