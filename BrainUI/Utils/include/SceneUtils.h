#pragma once
#include <qgraphicsscene.h>


inline void addItems(QGraphicsScene *scene, std::initializer_list<QGraphicsItem *> items) {
    for (auto *item : items) {
        scene->addItem(item);
    }
}
