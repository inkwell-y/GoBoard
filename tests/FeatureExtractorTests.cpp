#include <juce_core/juce_core.h>

#include "core/BoardState.h"
#include "core/FeatureExtractor.h"

class MacroFeatureExtractorTests final : public juce::UnitTest
{
public:
    MacroFeatureExtractorTests()
        : juce::UnitTest("MacroFeatureExtractor", "GoBoardController")
    {
    }

    void runTest() override
    {
        beginTest("empty board produces one-hot empty features");

        BoardState emptyBoard;
        const auto emptyFeatures = MacroFeatureExtractor::extract(emptyBoard);

        expectWithinAbsoluteError(emptyFeatures.getValue(MacroFeatureExtractor::Channel::Empty, 0, 0), 1.0f, 0.0001f);
        expectWithinAbsoluteError(emptyFeatures.getValue(MacroFeatureExtractor::Channel::Black, 0, 0), 0.0f, 0.0001f);
        expectWithinAbsoluteError(emptyFeatures.getValue(MacroFeatureExtractor::Channel::White, 0, 0), 0.0f, 0.0001f);

        beginTest("stones are encoded in separate black and white planes");

        BoardState board;
        board.setCell(1, 2, BoardState::CellState::Black);
        board.setCell(7, 8, BoardState::CellState::White);

        const auto features = MacroFeatureExtractor::extract(board);

        expectWithinAbsoluteError(features.getValue(MacroFeatureExtractor::Channel::Empty, 1, 2), 0.0f, 0.0001f);
        expectWithinAbsoluteError(features.getValue(MacroFeatureExtractor::Channel::Black, 1, 2), 1.0f, 0.0001f);
        expectWithinAbsoluteError(features.getValue(MacroFeatureExtractor::Channel::White, 1, 2), 0.0f, 0.0001f);

        expectWithinAbsoluteError(features.getValue(MacroFeatureExtractor::Channel::Empty, 7, 8), 0.0f, 0.0001f);
        expectWithinAbsoluteError(features.getValue(MacroFeatureExtractor::Channel::Black, 7, 8), 0.0f, 0.0001f);
        expectWithinAbsoluteError(features.getValue(MacroFeatureExtractor::Channel::White, 7, 8), 1.0f, 0.0001f);
    }
};

static MacroFeatureExtractorTests macroFeatureExtractorTests;
