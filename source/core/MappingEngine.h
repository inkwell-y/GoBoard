#pragma once

#include "BoardState.h"

class MappingEngine
{
public:
    struct MacroValues
    {
        float density = 0.0f;
        float balance = 0.5f;
        float centerX = 0.5f;
        float centerY = 0.5f;
        float edge = 0.0f;
        float conflict = 0.0f;
        float symmetry = 0.0f;
        float cluster = 0.0f;
    };

    static MacroValues mapBoardState(const BoardState& boardState) noexcept;

private:
    static float clamp01(float value) noexcept;
};
