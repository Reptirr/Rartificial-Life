#include <BrainMap.h>
#include <Dendrite.h>

std::vector<std::string> BrainMap::searchDendrite(BrainPoint from, double r) {
    std::vector<std::string> res{};

    double x_min = from.x-r;
    double x_max = from.x+r;
    double y_min = from.y-r;
    double y_max = from.y+r;

    for (auto &dendrite_id : storage_->dendrites()) {
        auto *dendrite = storage_->get<Dendrite>(dendrite_id);
        auto p = dendrite->point(); // тут

        if (x_min < p.x && p.x < x_max
            && y_min < p.y && p.y < y_max) {
            res.push_back(dendrite_id);
        }
    }

    return res;
}
