#include "RNG.hpp"
#include <random>

namespace Probabilities {

// Génère un nombre aléatoire dans un intervalle donné. Utile pour des positions aléatoires.
double RNG::random_0_1()
{
    std::random_device               rd;
    std::mt19937                     gen(rd());
    std::uniform_real_distribution<> dis(0, 1);
    return dis(gen);
}

bool RNG::bernoulli(double p)
{
    if (p < 0 || p > 1)
    {
        throw std::invalid_argument("Probability must be between 0 and 1");
    }
    return random_0_1() < p;
}

// binomial distribution (used for combo catch)
int RNG::binomial(int n, double p)
{
    if (p < 0 || p > 1 || n < 0)
    {
        throw std::invalid_argument("Invalid parameters for binomial distribution");
    }
    int successes = 0;
    for (int i = 0; i < n; i++)
    {
        if (bernoulli(p))
        {
            successes++;
        }
    }
    return successes;
}

// Loi géométrique (nb d’échecs avant premier succès)
int RNG::geometric(double p)
{
    if (p <= 0 || p >= 1)
        throw std::invalid_argument("Geometric p must be in (0,1)");
    return std::floor(std::log(1.0 - random_0_1()) / std::log(1.0 - p));
}

// Normal distribution using Box-Muller transform
// Dégâts de collision lorsqu'une pièce arrive à côté d'une pièce adverse.
double RNG::normal(double mean, double sigma)
{
    double u1 = random_0_1();
    double u2 = random_0_1();
    double z0 = std::sqrt(-2.0 * std::log(u1)) * std::cos(2.0 * M_PI * u2);
    return mean + sigma * z0;
}

// Temps de l'instabilité d'un pion. Si le joueur ne parvient pas à cliquer sur la pièce dans le temps imparti, elle peut : mourir, se transformer en une autre pièce ou un pion ennemi
double RNG::exponential(double lambda)
{
    if (lambda <= 0)
    {
        throw std::invalid_argument("Lambda must be positive");
    }
    return -std::log(1.0 - random_0_1()) / lambda; // time of the instability
}

// poisson distribution (used for player time reflexion)
double RNG::poisson(double lambda)
{
    double L = std::exp(-lambda);
    double p = 1.0;
    int    k = 0;

    do
    {
        k++;
        p *= random_0_1();
    } while (p > L);

    return k - 1;
}

double RNG::triangular(double a, double b, double c)
{
    if (a > b || c < a || c > b)
        throw std::invalid_argument("Invalid parameters for triangular distribution");

    double u = random_0_1();
    double F = (c - a) / (b - a);
    if (u < F)
        return a + std::sqrt(u * (b - a) * (c - a));
    else
        return b - std::sqrt((1 - u) * (b - a) * (b - c));
}

PowerUp RNG::sample_powerup()
{
    // Distribution arbitraire, ajustable
    std::vector<double> probs = {0.5, 0.2, 0.2, 0.1};
    double              u     = random_0_1();
    double              sum   = 0.0;
    for (size_t i = 0; i < probs.size(); ++i)
    {
        sum += probs[i];
        if (u < sum)
            return static_cast<PowerUp>(i);
    }
    return PowerUp::NONE; // fallback
}

} // namespace Probabilities
