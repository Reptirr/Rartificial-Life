#pragma once
#include <qpoint.h>

struct BrainPoint {
    double x{}, y{};

    BrainPoint mapTo(int w, int h) {
        auto x1 = (x+1) * (w/2);
        auto y1 = (y+1) * (h/2);

        return {x1,y1};
    }

    BrainPoint operator+(const BrainPoint& o) const { return {x + o.x, y + o.y}; }
    BrainPoint operator-(const BrainPoint& o) const { return {x - o.x, y - o.y}; }
    BrainPoint operator*(double k) const            { return {x * k, y * k}; }
    BrainPoint operator/(double k) const            { return {x / k, y / k}; }

    BrainPoint& operator+=(const BrainPoint& o) { x += o.x; y += o.y; return *this; }
    BrainPoint& operator-=(const BrainPoint& o) { x -= o.x; y -= o.y; return *this; }
    BrainPoint& operator*=(double k)            { x *= k; y *= k; return *this; }

    bool operator==(const BrainPoint& o) const { return x == o.x && y == o.y; }
    bool operator!=(const BrainPoint& o) const { return !(*this == o); }
};

// число слева: 2.0 * p
inline BrainPoint operator*(double k, const BrainPoint& p) { return p * k; }

[[nodiscard]] inline QPoint toQPoint(const BrainPoint& p) {
    return {static_cast<int>(std::lround(p.x)),
            static_cast<int>(std::lround(p.y))};
}

inline double dist(BrainPoint p1, BrainPoint p2) {
    return std::hypot(p2.x - p1.x, p2.y - p1.y);
}

inline BrainPoint middleBetween(BrainPoint p1, BrainPoint p2) {
    BrainPoint res{};

    res.x = (p1.x + p2.x) / 2;
    res.y = (p1.y + p2.y) / 2;

    return res;
}