#include <juce_core/juce_core.h>

#include "core/BoardState.h"
#include "core/FeatureExtractor.h"

class FeatureExtractorTests final : public juce::UnitTest
{
public:
    FeatureExtractorTests()
        : juce::UnitTest("FeatureExtractor", "GoBoardController")
    {
    }

    void runTest() override
    {
        beginTest("empty board produces one-hot empty features");

        BoardState emptyBoard;
        const auto emptyFeatures = FeatureExtractor::extract(emptyBoard);

        expectWithinAbsoluteError(emptyFeatures.getValue(FeatureExtractor::Channel::Empty, 0, 0), 1.0f, 0.0001f);
        expectWithinAbsoluteError(emptyFeatures.getValue(FeatureExtractor::Channel::Black, 0, 0), 0.0f, 0.0001f);
        expectWithinAbsoluteError(emptyFeatures.getValue(FeatureExtractor::Channel::White, 0, 0), 0.0f, 0.0001f);

        beginTest("stones are encoded in separate black and white planes");

        BoardState board;
        board.setCell(1, 2, BoardState::CellState::Black);
        board.setCell(7, 8, BoardState::CellState::White);

        const auto features = FeatureExtractor::extract(board);

        expectWithinAbsoluteError(features.getValue(FeatureExtractor::Channel::Empty, 1, 2), 0.0f, 0.0001f);
        expectWithinAbsoluteError(features.getValue(FeatureExtractor::Channel::Black, 1, 2), 1.0f, 0.0001f);
        expectWithinAbsoluteError(features.getValue(FeatureExtractor::Channel::White, 1, 2), 0.0f, 0.0001f);

        expectWithinAbsoluteError(features.getValue(FeatureExtractor::Channel::Empty, 7, 8), 0.0f, 0.0001f);
        expectWithinAbsoluteError(features.getValue(FeatureExtractor::Channel::Black, 7, 8), 0.0f, 0.0001f);
        expectWithinAbsoluteError(features.getValue(FeatureExtractor::Channel::White, 7, 8), 1.0f, 0.0001f);
    }
};

static FeatureExtractorTests featureExtractorTests;
