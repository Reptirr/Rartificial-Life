#pragma once
#include <NeuronItem.h>
#include <SynapsItem.h>
#include <UIComponent.h>

inline QPainterPath smoothPath(const QVector<QPointF> &pts) {
    QPainterPath path;
    const int n = pts.size();
    if (n == 0) return path;

    path.moveTo(pts[0]);
    if (n == 1) return path;
    if (n == 2) { path.lineTo(pts[1]); return path; }

    for (int i = 0; i < n - 1; ++i) {
        // соседние точки; на краях берём саму крайнюю точку
        const QPointF p0 = pts[qMax(i - 1, 0)];
        const QPointF p1 = pts[i];
        const QPointF p2 = pts[i + 1];
        const QPointF p3 = pts[qMin(i + 2, n - 1)];

        const QPointF c1 = p1 + (p2 - p0) / 6.0;
        const QPointF c2 = p2 - (p3 - p1) / 6.0;

        path.cubicTo(c1, c2, p2);
    }
    return path;
}

class AxonItem : public QGraphicsPathItem, public UIComponent {
    AxonData data_;

public:
    explicit AxonItem(UIStorage &storage, const AxonData &data) : UIComponent(storage), data_(data) {
        setZValue(UIConfig::axon_z);

        setPen({QColorConstants::Green, UIConfig::axon_width});
    }

    void UIUpdate() override {
        QVector<QPointF> pts{};
        for (auto node : data_.nodes) {
            pts.push_back(node.pos);
            if (!node.synaps.empty()) {
                pts.push_back(storage_.getSynaps(node.synaps)->getData().pos);
            }
        }

        const auto path = smoothPath(pts);

        setPath(path);
    }

    void setData(const AxonData &data) {
        data_ = data;
    }
};