#pragma once

#include <random>

namespace arkanoid::utils
{
    class Random
    {
    public:
        static bool RollChance(float a_fChance);

    private:
        static std::mt19937 m_RandomEngine;
    };
}