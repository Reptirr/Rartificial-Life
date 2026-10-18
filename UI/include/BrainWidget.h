#pragma once

#include <Dendrite.h>
#include <Neuron.h>
#include <QWidget>
#include <Storage.h>
#include <QTextItem>
#include <Synaps.h>

// px
static qreal neuron_r = 5;
static qreal synaps_r = 3;


class BrainWidget : public QWidget {
    Storage &storage_;

protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter p{this};
        p.setRenderHints(QPainter::Antialiasing);

        auto scr = [&](BrainPoint bp) {
            return toQPointF(bp.mapTo(width(), height()));
        };

        // рисуем нейроны и их аксоны
        for (auto neuron_id : storage_.neurons()) {
            auto *neuron = storage_.get<Neuron>(neuron_id);
            auto *axon = storage_.get<Axon>(neuron->getAxon());

            QPen pen{};
            pen.setColor(QColorConstants::Black);
            pen.setWidth(1);
            p.setPen(pen);

            QBrush brush{};
            brush.setColor(Qt::darkCyan);
            brush.setStyle(Qt::BrushStyle::SolidPattern);
            p.setBrush(brush);

            p.drawEllipse(scr(neuron->point()), neuron_r, neuron_r);

            // аксон
            p.setPen(QColorConstants::Green);
            // пробегаемся по каждой ноде аксона, строя линии
            for (auto &node : axon->nodes()) {
                p.drawPoint(scr(node.point));
                if (node.previous_node >= 0) {
                    p.drawLine(
                        scr(axon->nodes().at(node.previous_node).point),
                        scr(node.point)
                    );
                }

                if (auto &synaps_id = node.synaps; !synaps_id.empty()) {
                    auto *synaps = storage_.get<Synaps>(synaps_id);
                    auto med = synaps->getMediator();
                    auto Ca = synaps->getCa();
                    auto w = synaps->getW();

                    // рисуем линию к синапсу
                    p.drawLine(scr(node.point), scr(synaps->point()));

                    // рисуем синапс
                    p.setPen(QPen(Qt::gray, 2));
                    p.setBrush(Qt::gray);
                    p.drawEllipse(scr(synaps->point()), synaps_r, synaps_r);
                    // p.drawText(scr(synaps->point()), QString("Med: %1;  Ca: %2;  W: %3")
                    //                                 .arg(med, 0, 'f', 2)
                    //                                 .arg(Ca,  0, 'f', 2)
                    //                                 .arg(w,   0, 'f', 3));

                    // рисуем дендрит
                    auto dendrite_id = synaps->getDendrite();
                    auto *dendrite = storage_.get<Dendrite>(dendrite_id);

                    auto neuron2_id = dendrite->getNeuron();
                    auto *neuron2 = storage_.get<Neuron>(neuron2_id);

                    p.setPen(QPen(Qt::red, 2));
                    p.drawLine(scr(synaps->point()), scr(neuron2->point()));
                }
            }
        }
    }

public:
    explicit BrainWidget(Storage &storage, QWidget *parent = nullptr)
        : QWidget(parent),
          storage_(storage) {

    }

};
