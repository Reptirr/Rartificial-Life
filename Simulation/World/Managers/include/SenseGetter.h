#pragma once
#include <qline.h>
#include <WorldStorage.h>

class QColor;

class SenseGetter {
    WorldStorage &storage_;

    std::vector<QLineF> polygonEdges(const QPolygonF& poly) const;
    static QPointF pointByAzimuth(const QPointF from, const double azimuth, const double dist);
    static QLineF rayByAzimuth(QPointF from, const double azimuth, const double dist);

public:
    explicit SenseGetter(WorldStorage &storage)
        : storage_(storage) {
    }


    std::vector<QColor> getVision(
        QPointF eye,
        double azimuth,
        double fov,
        int rays,
        float maxDist,
        QColor default_color, const std::string &exclude_obj
    ) const;
};
