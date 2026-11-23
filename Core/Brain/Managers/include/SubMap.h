#pragma once
#include <BrainPoint.h>
#include <Genome.h>
#include <unordered_map>
#include <UUIDGenerator.h>
#include <Uuid.h>

class Storage;

class SubMap {
    Storage &storage_;
    std::unordered_map<Uuid, Protein *, UuidHash>& subs_;

public:
    explicit SubMap(Storage &storage);

    std::vector<Protein *> subsAtPoint(BrainPoint p) const;
};
