#pragma once

#include <random>

namespace arkanoid::utils
{
    class Random
    {
    public:
        static bool RollChance(float a_fChance);
		static int RandomInt(int a_iMin, int a_iMax);

    private:
        static std::mt19937 m_RandomEngine;
    };
}