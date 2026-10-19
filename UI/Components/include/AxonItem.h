#pragma once
#include <Axon.h>
#include <BrainPoint.h>
#include <NeuronItem.h>
#include <Synaps.h>
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
    Axon *axon_;

public:
    explicit AxonItem(Storage &storage, Axon *axon) : UIComponent(storage) {
        axon_ = axon;

        setZValue(UIConfig::axon_z);

        setPen({QColorConstants::Green, UIConfig::axon_width});
    }

    void UIUpdate() override {
        auto scr = [](BrainPoint p) { return toQPointF(p); };

        QVector<QPointF> pts{};
        for (auto node : axon_->nodes()) {
            pts.push_back(scr(node.point));
            if (!node.synaps.empty()) {
                pts.push_back(scr(storage_.get<Synaps>(node.synaps)->point()));
            }
        }

        auto path = smoothPath(pts);

        setPath(path);
    }
};