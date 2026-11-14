#pragma once
#include <Genome.h>
#include <RandomGenerator.h>

struct GenMinMax {
    double min{};
    double max{};
};

struct GenomeRngConfig {
    double real_step{0.1};
    int int_step{2};
    double gen_mutate_chance{0.1};

    int move_proteins_per_genblock{3};
    int avoid_proteins_per_genblock{3};

    std::unordered_map<std::string, GenMinMax> genome_min_max = {
        // neuron
        {Gen::NEURON_THRESHOLD,         {1.0,   50.0}},
        {Gen::NEURON_RANDOM_SPIKE_TICK, {0.0,   0.0}},  // 0 = шумовых спайков нет
        {Gen::NEURON_REFRACTOR_TICK,    {1.0,   20.0}},

        // axon
        {Gen::AXON_ANGLE_DEVIATION,     {0.0,   1.5}},    // рад, примерно до 90°
        {Gen::AXON_DEFAULT_STEP,        {0.01,  0.2}},
        {Gen::AXON_STEP_DEVIATION,      {0.0,   0.01}},   // не больше min шага
        {Gen::AXON_SEARCH_DENDRITE_R,   {0.005, 0.1}},
        {Gen::AXON_MAX_NODES,           {5.0,   200.0}},

        // synaps
        {Gen::SYNAPS_W_REDUCE_LR,       {0.0,   0.1}},
        {Gen::SYNAPS_W_PRODUCE_LR,      {0.0,   0.1}},
        {Gen::SYNAPS_W_MIN,             {-20.0,  0.0}},
        {Gen::SYNAPS_W_MAX,             {0.0,   20.0}},
        {Gen::SYNAPS_W_INIT,            {-2.0,  2.0}},
        {Gen::SYNAPS_CA_DECAY,          {0.01,  0.5}},
        {Gen::SYNAPS_MED_REDUCE_VAL,    {0.05,  2.0}},
    };
};

class GenomeRandomizer {
    static std::string mutateGen(const GenomeRngConfig &conf,
                                 const std::string &name,
                                 const std::string &val_s) {
        double val = std::stod(val_s);

        auto it = conf.genome_min_max.find(name);
        if (it == conf.genome_min_max.end())
            my_assert(false);

        const auto &[lo, hi] = it->second;
        double sigma = conf.real_step * (hi - lo);

        return std::to_string(std::clamp(Rng::normal(val, sigma), lo, hi));
    }

public:
    static Genome randomizing(GenomeRngConfig conf = GenomeRngConfig{},
                              int proteins = 50,
                              int genblocks = 20,
                              int gens_per_genblock = Gen::count() / 2) {
        const double &real_step = conf.real_step;
        const int &int_step = conf.int_step;

        Genome out = {
            .proteins = {},
            .genblocks = {DefaultGenblock::get}
        };

        for (int i = 0; i < proteins; i++) {
            Protein protein;

            protein.id = generateUuid();
            do {
                protein.dist = Rng::normal(0.1, real_step);
            } while (protein.dist <= 0);
            protein.genblock_i = Rng::randInt(genblocks ? 1 : 0, genblocks);
            protein.point = {
                .x = Rng::normal(Rng::randReal(-1.0, 1.0), real_step),
                .y = Rng::normal(Rng::randReal(-1.0, 1.0), real_step)
            };

            out.proteins.push_back(protein);
        }

        for (int i = 0; i < genblocks; i++) {
            GenBlock genblock;

            // количество генов считаем один раз, а не заново на каждой итерации цикла
            int gens_count = std::clamp(
                Rng::normal(gens_per_genblock, int_step),
                1, Gen::count());

            for (int gen_i = 0; gen_i < gens_count; gen_i++) {
                auto genname = Gen::fromInt(Rng::randInt(0, Gen::count() - 1));

                genblock.insert({
                    genname,
                    mutateGen(conf, genname, DefaultGenblock::get.at(genname))
                });
            }

            // move/avoid proteins
            my_assert(conf.move_proteins_per_genblock + conf.avoid_proteins_per_genblock < proteins);
            for (int j = 0; j < conf.move_proteins_per_genblock; j++) {
                std::string protein_id;
                do {
                    protein_id = out.proteins.at(Rng::randInt(0, proteins-1)).id;
                } while (genblock.axon_move_proteins.contains(protein_id) || genblock.axon_avoid_proteins.contains(protein_id));

                genblock.axon_move_proteins.insert(protein_id);
            }
            for (int j = 0; j < conf.avoid_proteins_per_genblock; j++) {
                std::string protein_id;
                do {
                    protein_id = out.proteins.at(Rng::randInt(0, proteins-1)).id;
                } while (genblock.axon_move_proteins.contains(protein_id) || genblock.axon_avoid_proteins.contains(protein_id));

                genblock.axon_avoid_proteins.insert(protein_id);
            }

            out.genblocks.push_back(genblock);
        }

        return out;
    }

    // ---------- randomize (мутация) ----------

    static Genome randomize(const Genome &src, GenomeRngConfig conf = GenomeRngConfig{}) {
        const double &real_step = conf.real_step;
        const int &int_step = conf.int_step;
        const double gen_mutate_chance = conf.gen_mutate_chance;

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
                out_protein.genblock_i = std::clamp(
                    Rng::normal(out_protein.genblock_i, int_step),
                    0, static_cast<int>(src.genblocks.size()) - 1);   // было proteins.size()

            out.proteins.push_back(out_protein);
        }

        // genblocks
        for (const auto &src_genblock : src.genblocks) {
            GenBlock out_genblock = src_genblock;

            for (auto &[name, val_s] : out_genblock) {
                if (!Rng::chance(gen_mutate_chance)) continue;
                val_s = mutateGen(conf, name, val_s);
            }

            out.genblocks.push_back(out_genblock);
        }

        return out;
    }
};
