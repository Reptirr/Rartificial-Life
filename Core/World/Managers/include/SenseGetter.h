#pragma once
#include <qline.h>
#include <WorldStorage.h>
#include <Uuid.h>

class QPolygonF;
class QColor;

class SenseGetter {
    WorldStorage &storage_;

    std::vector<QLineF> polygonEdges(const QPolygonF& poly) const;
    static QPointF pointByAzimuth(QPointF from, double azimuth, double dist);
    static QLineF rayByAzimuth(QPointF from, double azimuth, const double dist);

public:
    explicit SenseGetter(WorldStorage &storage)
        : storage_(storage) {
    }


    std::vector<QColor> getVision(
        const Uuid &id,
        double fov,
        int rays,
        float maxDist, QColor default_color, const Uuid &exclude_obj
    ) const;
};
