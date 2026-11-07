#include "./Random.h"

namespace arkanoid::utils
{
    std::mt19937 Random::m_RandomEngine{ std::random_device{}() };

    bool Random::RollChance(float a_fChance)
    {
        std::bernoulli_distribution distribution(a_fChance);
        return distribution(m_RandomEngine);
    }
}