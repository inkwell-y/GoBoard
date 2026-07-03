#pragma once

#include <array>
#include <cstddef>

struct ControlVector
{
    static constexpr std::size_t size = 8;

    enum Index : std::size_t
    {
        blackStoneDensity = 0,
        whiteStoneDensity,
        occupiedDensity,
        centerAreaDensity,
        topHalfDensity,
        bottomHalfDensity,
        leftHalfDensity,
        rightHalfDensity
    };

    std::array<float, size> values {};

    float operator[](std::size_t index) const noexcept
    {
        return values[index];
    }

    float& operator[](std::size_t index) noexcept
    {
        return values[index];
    }
};
