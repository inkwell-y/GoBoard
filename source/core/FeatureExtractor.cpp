#include "FeatureExtractor.h"

float FeatureExtractor::BoardFeatures::getValue(Channel channel, int row, int column) const noexcept
{
    return values[static_cast<std::size_t>(getIndex(channel, row, column))];
}

FeatureExtractor::BoardFeatures FeatureExtractor::extract(const BoardState& boardState) noexcept
{
    BoardFeatures features;

    for (int row = 0; row < BoardState::boardSize; ++row)
    {
        for (int column = 0; column < BoardState::boardSize; ++column)
        {
            const auto cell = boardState.getCell(row, column);

            const auto emptyIndex = static_cast<std::size_t>(BoardFeatures::getIndex(Channel::Empty, row, column));
            const auto blackIndex = static_cast<std::size_t>(BoardFeatures::getIndex(Channel::Black, row, column));
            const auto whiteIndex = static_cast<std::size_t>(BoardFeatures::getIndex(Channel::White, row, column));

            features.values[emptyIndex] = cell == BoardState::CellState::Empty ? 1.0f : 0.0f;
            features.values[blackIndex] = cell == BoardState::CellState::Black ? 1.0f : 0.0f;
            features.values[whiteIndex] = cell == BoardState::CellState::White ? 1.0f : 0.0f;
        }
    }

    return features;
}
