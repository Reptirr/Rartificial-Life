#pragma once

#include <random>

class Rng {
    static std::mt19937 &engine() {
        static std::mt19937 gen;
        return gen;
    }

public:
    static double normal(double mean, double sigma) {
        std::normal_distribution dist(mean, sigma);
        return dist(engine());
    }
    static int normal(int mean, int sigma) {
        std::normal_distribution dist(static_cast<double>(mean), static_cast<double>(sigma));
        return static_cast<int>(std::lround(dist(engine())));
    }

    static bool chance(double p) {
        std::bernoulli_distribution dist(p);
        return dist(engine());
    }

    static int randInt(const int from, const int to) {
        std::uniform_int_distribution dist(from, to);
        return dist(engine());
    }

    static double randReal(const double from, const double to) {
        std::uniform_real_distribution dist(from, to);
        return dist(engine());
    }

    static void seed(unsigned int s) {
        engine().seed(s);
    }
};
