#pragma once
#include <EntityKind.h>
#include <WorldEntity.h>

class Food : public WorldEntity {
public:
    EntityKind kind() const override {
        return EntityKind::FOOD;
    }
};
