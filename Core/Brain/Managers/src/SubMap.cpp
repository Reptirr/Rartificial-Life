#include <Storage.h>
#include <SubMap.h>

SubMap::SubMap(Storage &storage): storage_(storage), subs_(storage_.proteins()) {
}

std::vector<Protein *> SubMap::subsAtPoint(BrainPoint p) const {
    std::vector<Protein *> res;

    for (const auto &[id, sub] : subs_) {
        const auto dx = sub->point.x - p.x;
        const auto dy = sub->point.y - p.y;
        const auto r = sub->dist;

        if (dx * dx + dy * dy <= r * r) {
            res.push_back(sub);
        }
    }

    return res;
}
