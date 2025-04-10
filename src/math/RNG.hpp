#pragma once
#include <glm/glm.hpp>

namespace Probabilities {

enum class PowerUp {
    NONE,
    ROOK_POWER,
    BISHOP_POWER,
    QUEEN_POWER
};

// RNG
struct RNG {
    double random_0_1();
    bool   bernoulli(double p);       // explosion zone probability
    int    binomial(int n, double p); // combo catch probability
    int    geometric(double p);
    double normal(double mean, double sigma);
    double exponential(double lambda); // time of the instability
    double poisson(double lambda);
    double triangular(double a, double b, double c);
    //
    PowerUp sample_powerup();
};

} // namespace Probabilities
