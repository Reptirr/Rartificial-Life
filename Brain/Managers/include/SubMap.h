#pragma once
#include <BrainPoint.h>
#include <Genome.h>
#include <Storage.h>
#include <string>
#include <unordered_map>
#include <UUIDGenerator.h>


class SubMap {
    Storage &storage_;
    std::unordered_map<std::string, Protein *>& subs_;

public:
    explicit SubMap(Storage &storage)
        : storage_(storage), subs_(storage_.proteins()) {
    }

    std::vector<std::string> subsAtPoint(BrainPoint p) const {
        std::vector<std::string> res;

        for (auto [id, sub] : subs_) {
            if (dist(sub->point, p) <= sub->dist) {
                res.push_back(id);
            }
        }

        return res;
    }
};
