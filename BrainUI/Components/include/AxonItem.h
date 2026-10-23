#pragma once
#include <NeuronItem.h>
#include <SynapsItem.h>
#include <UIComponent.h>
#include <QStyleOption>

inline QVector<QPainterPath> smoothChunks(const QVector<QPointF> &pts, int segsPerChunk = 12) {
    QVector<QPainterPath> out;
    const int n = pts.size();
    if (n < 2) return out;

    QPainterPath cur;
    for (int i = 0; i < n - 1; ++i) {
        if (i % segsPerChunk == 0) {
            if (!cur.isEmpty()) out.push_back(cur);
            cur = QPainterPath();
            cur.moveTo(pts[i]);
        }
        const QPointF p0 = pts[qMax(i - 1, 0)];
        const QPointF p1 = pts[i];
        const QPointF p2 = pts[i + 1];
        const QPointF p3 = pts[qMin(i + 2, n - 1)];

        cur.cubicTo(p1 + (p2 - p0) / 6.0, p2 - (p3 - p1) / 6.0, p2);
    }
    out.push_back(cur);
    return out;
}

class AxonItem : public QGraphicsItem, public UIComponent {
    AxonData data_;
    QVector<QPainterPath> chunks_;
    QVector<QRectF> rects_;
    QRectF bounds_;
    QPen pen_{QColorConstants::Green, UIConfig::axon_width};

public:
    explicit AxonItem(UIStorage &storage, const AxonData &data) : UIComponent(storage), data_(data) {
        setZValue(UIConfig::axon_z);
        setFlag(ItemUsesExtendedStyleOption);        // нужно, чтобы работал exposedRect
        setAcceptedMouseButtons(Qt::NoButton);       // не считать попадания мышью по пути
    }

    QRectF boundingRect() const override { return bounds_; }
    QPainterPath shape() const override { return {}; }

    void paint(QPainter *p, const QStyleOptionGraphicsItem *opt, QWidget *) override {
        p->setPen(pen_);
        p->setBrush(Qt::NoBrush);
        const double m = pen_.widthF();
        const QRectF view = opt->exposedRect.adjusted(-m, -m, m, m);
        for (int i = 0; i < chunks_.size(); ++i)
            if (rects_[i].intersects(view))
                p->drawPath(chunks_[i]);
    }

    void UIUpdate() override {
        QVector<QPointF> pts;
        for (auto &node : data_.nodes) {
            pts.push_back(node.pos);
            if (!node.synaps.empty())
                pts.push_back(storage_.getSynaps(node.synaps)->getData().pos);
        }

        prepareGeometryChange();
        chunks_ = smoothChunks(pts);
        rects_.clear();
        bounds_ = QRectF();
        for (auto &c : chunks_) {
            QRectF r = c.controlPointRect();
            rects_.push_back(r);
            bounds_ = bounds_.united(r);
        }
        const double m = pen_.widthF();
        bounds_.adjust(-m, -m, m, m);
    }

    void setData(Data *d) override {
        if (const auto *data = dynamic_cast<AxonData *>(d))
            data_ = *data;
    }
};