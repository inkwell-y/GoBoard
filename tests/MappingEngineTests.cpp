#include <juce_core/juce_core.h>

#include "core/BoardState.h"
#include "core/MappingEngine.h"

class MappingEngineTests final : public juce::UnitTest
{
public:
    MappingEngineTests()
        : juce::UnitTest("MappingEngine", "GoBoardController")
    {
    }

    void runTest() override
    {
        beginTest("empty board returns zero densities");

        BoardState emptyBoard;
        const auto emptyValues = MappingEngine::mapBoardState(emptyBoard);

        expectWithinAbsoluteError(emptyValues.blackStoneDensity, 0.0f, 0.0001f);
        expectWithinAbsoluteError(emptyValues.whiteStoneDensity, 0.0f, 0.0001f);
        expectWithinAbsoluteError(emptyValues.occupiedDensity, 0.0f, 0.0001f);
        expectWithinAbsoluteError(emptyValues.centerAreaDensity, 0.0f, 0.0001f);
        expectWithinAbsoluteError(emptyValues.topHalfDensity, 0.0f, 0.0001f);
        expectWithinAbsoluteError(emptyValues.bottomHalfDensity, 0.0f, 0.0001f);
        expectWithinAbsoluteError(emptyValues.leftHalfDensity, 0.0f, 0.0001f);
        expectWithinAbsoluteError(emptyValues.rightHalfDensity, 0.0f, 0.0001f);

        beginTest("single black stone updates top and left densities");

        BoardState blackBoard;
        blackBoard.setCell(0, 0, BoardState::CellState::Black);

        const auto blackValues = MappingEngine::mapBoardState(blackBoard);

        expectWithinAbsoluteError(blackValues.blackStoneDensity, 1.0f / 81.0f, 0.0001f);
        expectWithinAbsoluteError(blackValues.whiteStoneDensity, 0.0f, 0.0001f);
        expectWithinAbsoluteError(blackValues.occupiedDensity, 1.0f / 81.0f, 0.0001f);
        expectWithinAbsoluteError(blackValues.centerAreaDensity, 0.0f, 0.0001f);
        expectWithinAbsoluteError(blackValues.topHalfDensity, 1.0f / 36.0f, 0.0001f);
        expectWithinAbsoluteError(blackValues.bottomHalfDensity, 0.0f, 0.0001f);
        expectWithinAbsoluteError(blackValues.leftHalfDensity, 1.0f / 36.0f, 0.0001f);
        expectWithinAbsoluteError(blackValues.rightHalfDensity, 0.0f, 0.0001f);

        beginTest("single white stone updates bottom and right densities");

        BoardState whiteBoard;
        whiteBoard.setCell(8, 8, BoardState::CellState::White);

        const auto whiteValues = MappingEngine::mapBoardState(whiteBoard);

        expectWithinAbsoluteError(whiteValues.blackStoneDensity, 0.0f, 0.0001f);
        expectWithinAbsoluteError(whiteValues.whiteStoneDensity, 1.0f / 81.0f, 0.0001f);
        expectWithinAbsoluteError(whiteValues.occupiedDensity, 1.0f / 81.0f, 0.0001f);
        expectWithinAbsoluteError(whiteValues.centerAreaDensity, 0.0f, 0.0001f);
        expectWithinAbsoluteError(whiteValues.topHalfDensity, 0.0f, 0.0001f);
        expectWithinAbsoluteError(whiteValues.bottomHalfDensity, 1.0f / 36.0f, 0.0001f);
        expectWithinAbsoluteError(whiteValues.leftHalfDensity, 0.0f, 0.0001f);
        expectWithinAbsoluteError(whiteValues.rightHalfDensity, 1.0f / 36.0f, 0.0001f);

        beginTest("center stones contribute to center and occupied densities");

        BoardState centerBoard;
        centerBoard.setCell(4, 4, BoardState::CellState::Black);
        centerBoard.setCell(3, 5, BoardState::CellState::White);

        const auto centerValues = MappingEngine::mapBoardState(centerBoard);

        expectWithinAbsoluteError(centerValues.blackStoneDensity, 1.0f / 81.0f, 0.0001f);
        expectWithinAbsoluteError(centerValues.whiteStoneDensity, 1.0f / 81.0f, 0.0001f);
        expectWithinAbsoluteError(centerValues.occupiedDensity, 2.0f / 81.0f, 0.0001f);
        expectWithinAbsoluteError(centerValues.centerAreaDensity, 2.0f / 9.0f, 0.0001f);
        expectWithinAbsoluteError(centerValues.topHalfDensity, 1.0f / 36.0f, 0.0001f);
        expectWithinAbsoluteError(centerValues.bottomHalfDensity, 0.0f, 0.0001f);
        expectWithinAbsoluteError(centerValues.leftHalfDensity, 0.0f, 0.0001f);
        expectWithinAbsoluteError(centerValues.rightHalfDensity, 1.0f / 36.0f, 0.0001f);
    }
};

static MappingEngineTests mappingEngineTests;
