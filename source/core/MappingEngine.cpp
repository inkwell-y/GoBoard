#include "MappingEngine.h"

#include <algorithm>

namespace
{
constexpr int boardSize = BoardState::boardSize;
constexpr float maxEdgeDistance = static_cast<float>(boardSize / 2);

float normaliseCoordinate(int coordinate) noexcept
{
    return static_cast<float>(coordinate) / static_cast<float>(boardSize - 1);
}
}

MappingEngine::MacroValues MappingEngine::mapBoardState(const BoardState& boardState) noexcept
{
    MacroValues values;

    const auto occupiedCount = boardState.countOccupied();

    if (occupiedCount == 0)
        return values;

    const auto blackCount = boardState.countBlack();
    const auto whiteCount = boardState.countWhite();

    values.density = clamp01(static_cast<float>(occupiedCount) / static_cast<float>(BoardState::numCells));
    values.balance = clamp01(0.5f + (0.5f * static_cast<float>(blackCount - whiteCount) / static_cast<float>(occupiedCount)));

    float totalX = 0.0f;
    float totalY = 0.0f;
    float totalEdge = 0.0f;
    int occupiedNeighbourPairs = 0;
    int oppositeNeighbourPairs = 0;
    int sameColourNeighbourPairs = 0;
    int horizontalMirrorMatches = 0;
    int verticalMirrorMatches = 0;

    for (int row = 0; row < boardSize; ++row)
    {
        for (int column = 0; column < boardSize; ++column)
        {
            const auto cell = boardState.getCell(row, column);

            if (cell == BoardState::CellState::Empty)
                continue;

            totalX += normaliseCoordinate(column);
            totalY += normaliseCoordinate(row);

            const auto distanceToEdge = std::min({ row, column, (boardSize - 1) - row, (boardSize - 1) - column });
            totalEdge += 1.0f - (static_cast<float>(distanceToEdge) / maxEdgeDistance);

            if (boardState.getCell(row, boardSize - 1 - column) == cell)
                ++horizontalMirrorMatches;

            if (boardState.getCell(boardSize - 1 - row, column) == cell)
                ++verticalMirrorMatches;

            if (column + 1 < boardSize)
            {
                const auto neighbour = boardState.getCell(row, column + 1);

                if (neighbour != BoardState::CellState::Empty)
                {
                    ++occupiedNeighbourPairs;

                    if (neighbour == cell)
                        ++sameColourNeighbourPairs;
                    else
                        ++oppositeNeighbourPairs;
                }
            }

            if (row + 1 < boardSize)
            {
                const auto neighbour = boardState.getCell(row + 1, column);

                if (neighbour != BoardState::CellState::Empty)
                {
                    ++occupiedNeighbourPairs;

                    if (neighbour == cell)
                        ++sameColourNeighbourPairs;
                    else
                        ++oppositeNeighbourPairs;
                }
            }
        }
    }

    values.centerX = clamp01(totalX / static_cast<float>(occupiedCount));
    values.centerY = clamp01(totalY / static_cast<float>(occupiedCount));
    values.edge = clamp01(totalEdge / static_cast<float>(occupiedCount));
    values.symmetry = clamp01(0.5f * ((static_cast<float>(horizontalMirrorMatches) / static_cast<float>(occupiedCount))
                                    + (static_cast<float>(verticalMirrorMatches) / static_cast<float>(occupiedCount))));

    if (occupiedNeighbourPairs > 0)
    {
        const auto pairCount = static_cast<float>(occupiedNeighbourPairs);
        values.conflict = clamp01(static_cast<float>(oppositeNeighbourPairs) / pairCount);
        values.cluster = clamp01(static_cast<float>(sameColourNeighbourPairs) / pairCount);
    }

    return values;
}

float MappingEngine::clamp01(float value) noexcept
{
    return std::clamp(value, 0.0f, 1.0f);
}
