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
        beginTest("empty board returns neutral defaults");

        BoardState emptyBoard;
        const auto emptyValues = MappingEngine::mapBoardState(emptyBoard);

        expectWithinAbsoluteError(emptyValues.density, 0.0f, 0.0001f);
        expectWithinAbsoluteError(emptyValues.balance, 0.5f, 0.0001f);
        expectWithinAbsoluteError(emptyValues.centerX, 0.5f, 0.0001f);
        expectWithinAbsoluteError(emptyValues.centerY, 0.5f, 0.0001f);
        expectWithinAbsoluteError(emptyValues.edge, 0.0f, 0.0001f);
        expectWithinAbsoluteError(emptyValues.conflict, 0.0f, 0.0001f);
        expectWithinAbsoluteError(emptyValues.symmetry, 0.0f, 0.0001f);
        expectWithinAbsoluteError(emptyValues.cluster, 0.0f, 0.0001f);

        beginTest("adjacent opposite colours raise conflict and edge");

        BoardState conflictBoard;
        conflictBoard.setCell(0, 0, BoardState::CellState::Black);
        conflictBoard.setCell(0, 1, BoardState::CellState::White);

        const auto conflictValues = MappingEngine::mapBoardState(conflictBoard);

        expectWithinAbsoluteError(conflictValues.density, 2.0f / 81.0f, 0.0001f);
        expectWithinAbsoluteError(conflictValues.balance, 0.5f, 0.0001f);
        expectWithinAbsoluteError(conflictValues.centerX, 0.0625f, 0.0001f);
        expectWithinAbsoluteError(conflictValues.centerY, 0.0f, 0.0001f);
        expectWithinAbsoluteError(conflictValues.edge, 1.0f, 0.0001f);
        expectWithinAbsoluteError(conflictValues.conflict, 1.0f, 0.0001f);
        expectWithinAbsoluteError(conflictValues.cluster, 0.0f, 0.0001f);
        expectWithinAbsoluteError(conflictValues.symmetry, 0.0f, 0.0001f);

        beginTest("adjacent same colours raise cluster");

        BoardState clusterBoard;
        clusterBoard.setCell(4, 4, BoardState::CellState::Black);
        clusterBoard.setCell(4, 5, BoardState::CellState::Black);

        const auto clusterValues = MappingEngine::mapBoardState(clusterBoard);

        expectWithinAbsoluteError(clusterValues.balance, 1.0f, 0.0001f);
        expectWithinAbsoluteError(clusterValues.conflict, 0.0f, 0.0001f);
        expectWithinAbsoluteError(clusterValues.cluster, 1.0f, 0.0001f);

        beginTest("mirrored placement increases symmetry");

        BoardState symmetricBoard;
        symmetricBoard.setCell(2, 1, BoardState::CellState::Black);
        symmetricBoard.setCell(2, 7, BoardState::CellState::Black);

        const auto symmetricValues = MappingEngine::mapBoardState(symmetricBoard);

        expectWithinAbsoluteError(symmetricValues.centerX, 0.5f, 0.0001f);
        expectWithinAbsoluteError(symmetricValues.centerY, 0.25f, 0.0001f);
        expectWithinAbsoluteError(symmetricValues.symmetry, 0.5f, 0.0001f);
    }
};

static MappingEngineTests mappingEngineTests;
