#include <Storage.h>
#include <SubMap.h>

SubMap::SubMap(Storage &storage): storage_(storage), subs_(storage_.proteins()) {
}

std::vector<Protein *> SubMap::subsAtPoint(BrainPoint p) const {
    std::vector<Protein *> res;

    for (auto [id, sub] : subs_) {
        if (dist(sub->point, p) <= sub->dist) {
            res.push_back(sub);
        }
    }

    return res;
}
