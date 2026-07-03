#include "MappingEngine.h"

#include <algorithm>

namespace
{
constexpr int boardSize = BoardState::boardSize;
constexpr int centerStart = 3;
constexpr int centerEnd = 5;
constexpr int centerAreaCellCount = 9;
constexpr int halfAreaCellCount = 36;
}

MappingEngine::MacroValues MappingEngine::mapBoardState(const BoardState& boardState) noexcept
{
    MacroValues values;
    int centerAreaOccupiedCount = 0;
    int topHalfOccupiedCount = 0;
    int bottomHalfOccupiedCount = 0;
    int leftHalfOccupiedCount = 0;
    int rightHalfOccupiedCount = 0;

    for (int row = 0; row < boardSize; ++row)
    {
        for (int column = 0; column < boardSize; ++column)
        {
            const auto cell = boardState.getCell(row, column);

            if (cell == BoardState::CellState::Empty)
                continue;

            if (row >= centerStart && row <= centerEnd && column >= centerStart && column <= centerEnd)
                ++centerAreaOccupiedCount;

            if (row < boardSize / 2)
                ++topHalfOccupiedCount;
            else if (row > boardSize / 2)
                ++bottomHalfOccupiedCount;

            if (column < boardSize / 2)
                ++leftHalfOccupiedCount;
            else if (column > boardSize / 2)
                ++rightHalfOccupiedCount;
        }
    }

    values.blackStoneDensity = clamp01(static_cast<float>(boardState.countBlack()) / static_cast<float>(BoardState::numCells));
    values.whiteStoneDensity = clamp01(static_cast<float>(boardState.countWhite()) / static_cast<float>(BoardState::numCells));
    values.occupiedDensity = clamp01(static_cast<float>(boardState.countOccupied()) / static_cast<float>(BoardState::numCells));
    values.centerAreaDensity = clamp01(static_cast<float>(centerAreaOccupiedCount) / static_cast<float>(centerAreaCellCount));
    values.topHalfDensity = clamp01(static_cast<float>(topHalfOccupiedCount) / static_cast<float>(halfAreaCellCount));
    values.bottomHalfDensity = clamp01(static_cast<float>(bottomHalfOccupiedCount) / static_cast<float>(halfAreaCellCount));
    values.leftHalfDensity = clamp01(static_cast<float>(leftHalfOccupiedCount) / static_cast<float>(halfAreaCellCount));
    values.rightHalfDensity = clamp01(static_cast<float>(rightHalfOccupiedCount) / static_cast<float>(halfAreaCellCount));

    return values;
}

float MappingEngine::clamp01(float value) noexcept
{
    return std::clamp(value, 0.0f, 1.0f);
}
