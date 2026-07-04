#pragma once

#include "BoardState.h"

#include <array>

class MacroFeatureExtractor
{
public:
    enum class Channel
    {
        Empty = 0,
        Black,
        White
    };

    struct MacroFeatures
    {
        static constexpr int boardSize = BoardState::boardSize;
        static constexpr int channelCount = 3;
        static constexpr int valuesPerChannel = BoardState::numCells;
        static constexpr int numValues = channelCount * valuesPerChannel;

        std::array<float, numValues> values {};

        float getValue(Channel channel, int row, int column) const noexcept;

    private:
        static constexpr int getIndex(Channel channel, int row, int column) noexcept
        {
            return static_cast<int>(channel) * valuesPerChannel + row * boardSize + column;
        }

        friend class MacroFeatureExtractor;
    };

    static MacroFeatures extract(const BoardState& boardState) noexcept;
};
