#pragma once

#include "BoardState.h"

class MappingEngine
{
public:
    struct MacroValues
    {
        float blackStoneDensity = 0.0f;
        float whiteStoneDensity = 0.0f;
        float occupiedDensity = 0.0f;
        float centerAreaDensity = 0.0f;
        float topHalfDensity = 0.0f;
        float bottomHalfDensity = 0.0f;
        float leftHalfDensity = 0.0f;
        float rightHalfDensity = 0.0f;
    };

    static MacroValues mapBoardState(const BoardState& boardState) noexcept;

private:
    static float clamp01(float value) noexcept;
};
