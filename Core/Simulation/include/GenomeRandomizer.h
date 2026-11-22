#pragma once
#include <Genome.h>
#include <my_assert.h>
#include <RandomGenerator.h>
#include <UUIDGenerator.h>

struct GenMinMax {
    double min{};
    double max{};
};

struct GenomeRngConfig {
    double gen_sigma{0.1};
    double protein_sigma{0.1};
    double protein_p_sigma{0.05};
    double protein_dist_sigma{0.1};
    int genblock_gens_sigma{3};
    int protein_genblock_i_sigma{1};

    double protein_num_change_chance{0.2};
    int protein_num_sigma{1}; // при увеличении дублируется существующий. при уменьшении убираем рандомный

    double gen_mutate_chance{0.1};

    int protein_max_neurons{100};
    int protein_neurons_sigma{5};

    int move_proteins_per_genblock{3};
    int avoid_proteins_per_genblock{3};

    std::unordered_map<std::string, GenMinMax> genome_min_max = {
        // neuron
        {Gen::NEURON_THRESHOLD,         {1.0,   50.0}},
        {Gen::NEURON_RANDOM_SPIKE_TICK, {90.0,   100.0}},  // 0 = шумовых спайков нет
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
        double sigma = conf.gen_sigma * (hi - lo);

        return std::to_string(std::clamp(Rng::normal(val, sigma), lo, hi));
    }

public:
    static Genome randomizing(GenomeRngConfig conf = GenomeRngConfig{},
                              int proteins = 25,
                              int genblocks = 20,
                              int gens_per_genblock = Gen::count() / 2) {
        Genome out = {
            .proteins_num = proteins,
            .proteins = {},
            .genblocks = {DefaultGenblock::get}
        };

        for (int i = 0; i < proteins; i++) {
            Protein protein;

            protein.id = generateUuid();
            do {
                protein.dist = Rng::normal(0.1, conf.protein_sigma);
            } while (protein.dist <= 0);
            protein.genblock_i = Rng::randInt(genblocks ? 1 : 0, genblocks);
            protein.point = {
                .x = Rng::normal(Rng::randReal(-1.0, 1.0), conf.protein_p_sigma),
                .y = Rng::normal(Rng::randReal(-1.0, 1.0), conf.protein_p_sigma)
            };
            protein.neurons_num = Rng::randInt(10, conf.protein_max_neurons);

            out.proteins.push_back(protein);
        }

        for (int i = 0; i < genblocks; i++) {
            GenBlock genblock;

            // количество генов считаем один раз, а не заново на каждой итерации цикла
            int gens_count = std::clamp(
                Rng::normal(gens_per_genblock, conf.genblock_gens_sigma),
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
        const double gen_mutate_chance = conf.gen_mutate_chance;

        Genome out = src;

        // proteins num change
        if (Rng::chance(conf.protein_num_change_chance)) {
            auto change = Rng::normal(src.proteins_num, conf.protein_num_sigma);
            if (change > 0) {
                for (auto i = 0; i < change; i++) {
                    // дублируем существующий в рандомную точку
                    auto p = BrainPoint{Rng::randReal(-1, 1), Rng::randReal(-1, 1)};
                    auto src_protein = src.proteins.at(Rng::randInt(0, src.proteins_num-1));

                    auto new_protein = src_protein; // copy
                    new_protein.point = p;

                    out.proteins.push_back(new_protein);
                    out.proteins_num++;
                }
            }
            if (change < 0 && out.proteins.size() > 1) {
                change = std::abs(change); // для for
                for (auto i = 0; i < change; i++) {
                    // удаляем рандомный протеин
                    // последний i считается рандомным
                    out.proteins.pop_back();
                }
            }
        }

        // proteins
        for (auto &protein : out.proteins) {
            if (Rng::chance(gen_mutate_chance))
                protein.point = {
                    .x = Rng::normal(protein.point.x, conf.protein_p_sigma),
                    .y = Rng::normal(protein.point.y, conf.protein_p_sigma)
                };

            if (Rng::chance(gen_mutate_chance))
                protein.dist = Rng::normal(protein.dist, conf.protein_dist_sigma);

            if (Rng::chance(gen_mutate_chance))
                protein.genblock_i = std::clamp(
                    Rng::normal(protein.genblock_i, conf.protein_genblock_i_sigma),
                    0, static_cast<int>(src.genblocks.size()) - 1);   // было proteins.size()

            if (Rng::chance(gen_mutate_chance))
                protein.neurons_num = std::clamp(
                    Rng::normal(protein.neurons_num, conf.protein_neurons_sigma),
                    10, conf.protein_max_neurons
                );
        }

        // genblocks
        for (auto &genblock : out.genblocks) {
            for (auto &[name, val_s] : genblock) {
                if (!Rng::chance(gen_mutate_chance)) continue;

                val_s = mutateGen(conf, name, val_s);
            }
        }

        return out;
    }
};
