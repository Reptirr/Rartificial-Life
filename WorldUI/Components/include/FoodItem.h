#pragma once
#include <WorldObjectItem.h>

class FoodItem : public WorldObjectItem {
public:
    explicit FoodItem(const WorldObjectData *data)
        : WorldObjectItem(data) {
    }
};