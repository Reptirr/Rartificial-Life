#pragma once

#include <Neuron.h>
#include <QWidget>
#include <Storage.h>
#include <QPainter>
#include <UIStorage.h>
#include <QTextItem>

class BrainWidget : public QWidget {
    Storage &storage_;
    UIStorage ui_storage_{};


    void drawLink(QPainter& p, const BrainLinkRef& link) {
        BrainPoint a = storage_.get<Neuron>(link.neuron1)->point();
        BrainPoint b = storage_.get<Neuron>(link.neuron2)->point();
        BrainPoint d = b - a;

        BrainPoint pAxon     = a + d * 0.75;
        BrainPoint pSynaps   = a + d * 0.75;
        BrainPoint pDendrite = a + d * 0.75;

        auto *synaps = storage_.get<Synaps>(link.synaps);
        double Ca = synaps->getCa();
        double med = synaps->getMediator();
        double w = synaps->getW();


        auto scr = [&](BrainPoint bp) {
            return toQPoint(bp.mapTo(width(), height()));
        };

        QFont font("Arial", 12);          // семейство и размер в пунктах
        font.setBold(true);                         // жирный
        font.setItalic(false);
        p.setFont(font);

        // аксон: зелёный
        p.setPen(QPen(Qt::green, 2));
        p.drawLine(scr(a), scr(pAxon));

        // синапс: серый
        p.setPen(QPen(Qt::gray, 2));
        p.setBrush(Qt::gray);
        p.drawEllipse(scr(pSynaps), 7, 7);
        p.drawText(scr(pSynaps), QString("Med: %1;  Ca: %2;  W: %3")
                                        .arg(med, 0, 'f', 2)
                                        .arg(Ca,  0, 'f', 2)
                                        .arg(w,   0, 'f', 3));

        // дендрит: красный
        p.setPen(QPen(Qt::red, 2));
        p.drawLine(scr(pDendrite), scr(b));
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter p{this};
        p.setRenderHints(QPainter::Antialiasing);

        // рисуем связи-синапсы
        for (auto synaps_id : storage_.synapses()) {
            auto *synaps = storage_.get<Synaps>(synaps_id);
            auto *axon = storage_.get<Axon>(synaps->getAxon());
            auto *dendrite = storage_.get<Dendrite>(synaps->getDendrite());

            auto axon_id = synaps->getAxon();
            auto dendrite_id = synaps->getDendrite();
            auto neuron1_id = axon->getNeuron();
            auto neuron2_id = dendrite->getNeuron();

            drawLink(p, {
                .neuron1 = neuron1_id,
                .axon = axon_id,
                .synaps = synaps_id,
                .dendrite = dendrite_id,
                .neuron2 = neuron2_id
            });
        }

        // рисуем нейроны
        for (auto neuron_id : storage_.neurons()) {
            auto *neuron = storage_.get<Neuron>(neuron_id);

            QPen pen{};
            pen.setColor(QColorConstants::Black);
            pen.setWidth(1);
            p.setPen(pen);

            QBrush brush{};
            brush.setColor(Qt::darkCyan);
            brush.setStyle(Qt::BrushStyle::SolidPattern);
            p.setBrush(brush);

            auto res = neuron->point().mapTo(width(), height());

            p.drawEllipse(QPoint(res.x, res.y), 14, 14);
        }
    }

public:
    explicit BrainWidget(Storage &storage, QWidget *parent = nullptr)
        : QWidget(parent),
          storage_(storage) {

    }

};
