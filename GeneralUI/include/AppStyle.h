#pragma once

#include <QProxyStyle>

class AppStyle : public QProxyStyle {
    std::unordered_map<PixelMetric, int> sizes = {
        {PM_ToolBarIconSize, 28}
    };
public:
    int pixelMetric(
        PixelMetric metric,
        const QStyleOption *option,
        const QWidget *widget
    ) const override {
        if (sizes.contains(metric)) return sizes.at(metric);
        return QProxyStyle::pixelMetric(metric, option, widget);
    }
};