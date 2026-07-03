#include "MappingEngine.h"

#include <algorithm>

namespace
{
constexpr int boardSize = FeatureExtractor::BoardFeatures::boardSize;
constexpr int centerStart = 3;
constexpr int centerEnd = 5;
constexpr int centerAreaCellCount = 9;
constexpr int halfAreaCellCount = 36;

bool isOccupied(const FeatureExtractor::BoardFeatures& features, int row, int column) noexcept
{
    return features.getValue(FeatureExtractor::Channel::Black, row, column) > 0.5f
        || features.getValue(FeatureExtractor::Channel::White, row, column) > 0.5f;
}

int countChannel(const FeatureExtractor::BoardFeatures& features, FeatureExtractor::Channel channel) noexcept
{
    int count = 0;

    for (int row = 0; row < boardSize; ++row)
    {
        for (int column = 0; column < boardSize; ++column)
        {
            if (features.getValue(channel, row, column) > 0.5f)
                ++count;
        }
    }

    return count;
}
}

ControlVector DeterministicMappingEngine::mapFeatures(const FeatureExtractor::BoardFeatures& features) const noexcept
{
    ControlVector values;
    int centerAreaOccupiedCount = 0;
    int topHalfOccupiedCount = 0;
    int bottomHalfOccupiedCount = 0;
    int leftHalfOccupiedCount = 0;
    int rightHalfOccupiedCount = 0;

    for (int row = 0; row < boardSize; ++row)
    {
        for (int column = 0; column < boardSize; ++column)
        {
            if (! isOccupied(features, row, column))
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

    const auto blackCount = countChannel(features, FeatureExtractor::Channel::Black);
    const auto whiteCount = countChannel(features, FeatureExtractor::Channel::White);
    const auto occupiedCount = blackCount + whiteCount;

    values[ControlVector::blackStoneDensity] = clamp01(static_cast<float>(blackCount) / static_cast<float>(BoardState::numCells));
    values[ControlVector::whiteStoneDensity] = clamp01(static_cast<float>(whiteCount) / static_cast<float>(BoardState::numCells));
    values[ControlVector::occupiedDensity] = clamp01(static_cast<float>(occupiedCount) / static_cast<float>(BoardState::numCells));
    values[ControlVector::centerAreaDensity] = clamp01(static_cast<float>(centerAreaOccupiedCount) / static_cast<float>(centerAreaCellCount));
    values[ControlVector::topHalfDensity] = clamp01(static_cast<float>(topHalfOccupiedCount) / static_cast<float>(halfAreaCellCount));
    values[ControlVector::bottomHalfDensity] = clamp01(static_cast<float>(bottomHalfOccupiedCount) / static_cast<float>(halfAreaCellCount));
    values[ControlVector::leftHalfDensity] = clamp01(static_cast<float>(leftHalfOccupiedCount) / static_cast<float>(halfAreaCellCount));
    values[ControlVector::rightHalfDensity] = clamp01(static_cast<float>(rightHalfOccupiedCount) / static_cast<float>(halfAreaCellCount));

    return values;
}

float DeterministicMappingEngine::clamp01(float value) noexcept
{
    return std::clamp(value, 0.0f, 1.0f);
}
