#pragma once
#include <Genome.h>
#include <random>
#include <RandomGenerator.h>
#include <ranges>

struct GenomeRngConfig {
    double real_step{0.1};
    int int_step{2};
    double gen_mutate_chance{0.1};

};

class GenomeRandomizer {
public:
    static Genome randomizing(GenomeRngConfig conf=GenomeRngConfig{}, int proteins=10, int genblocks=10, int gens_per_genblock=Gen::count()/2) {
        double &real_step = conf.real_step;
        int &int_step = conf.int_step;
        double gen_mutate_chance = conf.gen_mutate_chance;

        Genome out = {
            .proteins = {},
            .genblocks = {DefaultGenblock::get}
        };

        for (int i = 0; i < proteins; i++) {
            Protein protein;

            protein.dist = Rng::normal(0.2, real_step);
            protein.genblock_i = Rng::randInt(genblocks ? 1 : 0, genblocks);
            protein.point = {
                .x = Rng::normal(Rng::randReal(-1.0, 1.0), real_step),
                .y = Rng::normal(Rng::randReal(-1.0, 1.0), real_step)
            };

            out.proteins.push_back(protein);
        }

        for (int i = 0; i < genblocks; i++) {
            GenBlock genblock;

            for (int gen_i = 0; gen_i < Rng::normal(gens_per_genblock, int_step); gen_i++) {
                auto genname = Gen::fromInt(Rng::randInt(0, Gen::count()-1));
                auto val = Rng::normal(
                    std::stod(DefaultGenblock::get.at(genname)),
                    real_step
                );

                genblock.insert({genname, std::to_string(val)});
            }

            out.genblocks.push_back(genblock);
        }


        return out;
    }

    static Genome randomize(const Genome &src, GenomeRngConfig conf = GenomeRngConfig{}) {
        double &real_step = conf.real_step;
        int &int_step = conf.int_step;
        double gen_mutate_chance = conf.gen_mutate_chance;


        static std::random_device rng;
        static std::mt19937 gen{rng()};

        Genome out;

        // proteins
        for (const auto &src_protein : src.proteins) {
            Protein out_protein = src_protein;

            if (Rng::chance(gen_mutate_chance))
                out_protein.point = {
                    .x = Rng::normal(out_protein.point.x, real_step),
                    .y = Rng::normal(out_protein.point.y, real_step)
                };

            if (Rng::chance(gen_mutate_chance))
                out_protein.dist = Rng::normal(out_protein.dist, real_step);

            if (Rng::chance(gen_mutate_chance))
                out_protein.genblock_i = std::clamp(Rng::normal(out_protein.genblock_i, int_step), 0, (int)src.proteins.size()-1);

            out.proteins.push_back(out_protein);
        }

        // genblocks
        for (const auto &src_genblock : src.genblocks) {
            GenBlock out_genblock = src_genblock;

            for (auto &val_s : out_genblock | std::views::values) {
                if (!Rng::chance(gen_mutate_chance)) continue;

                auto val = std::stod(val_s);
                val = Rng::normal(val, real_step);
                val_s = std::to_string(val);
            }

            out.genblocks.push_back(out_genblock);
        }

        return out;
    }
};
