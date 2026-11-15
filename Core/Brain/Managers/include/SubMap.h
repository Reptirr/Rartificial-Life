#pragma once
#include <BrainPoint.h>
#include <Genome.h>
#include <string>
#include <unordered_map>
#include <UUIDGenerator.h>


class Storage;

class SubMap {
    Storage &storage_;
    std::unordered_map<std::string, Protein *>& subs_;

public:
    explicit SubMap(Storage &storage);

    std::vector<Protein *> subsAtPoint(BrainPoint p) const;
};
