#include <juce_core/juce_core.h>

#include "core/BoardState.h"
#include "core/ControlVector.h"
#include "core/FeatureExtractor.h"
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
        DeterministicMappingEngine mappingEngine;

        beginTest("empty board returns zero control values");

        BoardState emptyBoard;
        const auto emptyValues = mappingEngine.mapFeatures(FeatureExtractor::extract(emptyBoard));

        for (std::size_t index = 0; index < ControlVector::size; ++index)
            expectWithinAbsoluteError(emptyValues[index], 0.0f, 0.0001f);

        beginTest("single black stone updates top and left densities");

        BoardState blackBoard;
        blackBoard.setCell(0, 0, BoardState::CellState::Black);

        const auto blackValues = mappingEngine.mapFeatures(FeatureExtractor::extract(blackBoard));

        expectWithinAbsoluteError(blackValues[ControlVector::blackStoneDensity], 1.0f / 81.0f, 0.0001f);
        expectWithinAbsoluteError(blackValues[ControlVector::whiteStoneDensity], 0.0f, 0.0001f);
        expectWithinAbsoluteError(blackValues[ControlVector::occupiedDensity], 1.0f / 81.0f, 0.0001f);
        expectWithinAbsoluteError(blackValues[ControlVector::centerAreaDensity], 0.0f, 0.0001f);
        expectWithinAbsoluteError(blackValues[ControlVector::topHalfDensity], 1.0f / 36.0f, 0.0001f);
        expectWithinAbsoluteError(blackValues[ControlVector::bottomHalfDensity], 0.0f, 0.0001f);
        expectWithinAbsoluteError(blackValues[ControlVector::leftHalfDensity], 1.0f / 36.0f, 0.0001f);
        expectWithinAbsoluteError(blackValues[ControlVector::rightHalfDensity], 0.0f, 0.0001f);

        beginTest("single white stone updates bottom and right densities");

        BoardState whiteBoard;
        whiteBoard.setCell(8, 8, BoardState::CellState::White);

        const auto whiteValues = mappingEngine.mapFeatures(FeatureExtractor::extract(whiteBoard));

        expectWithinAbsoluteError(whiteValues[ControlVector::blackStoneDensity], 0.0f, 0.0001f);
        expectWithinAbsoluteError(whiteValues[ControlVector::whiteStoneDensity], 1.0f / 81.0f, 0.0001f);
        expectWithinAbsoluteError(whiteValues[ControlVector::occupiedDensity], 1.0f / 81.0f, 0.0001f);
        expectWithinAbsoluteError(whiteValues[ControlVector::centerAreaDensity], 0.0f, 0.0001f);
        expectWithinAbsoluteError(whiteValues[ControlVector::topHalfDensity], 0.0f, 0.0001f);
        expectWithinAbsoluteError(whiteValues[ControlVector::bottomHalfDensity], 1.0f / 36.0f, 0.0001f);
        expectWithinAbsoluteError(whiteValues[ControlVector::leftHalfDensity], 0.0f, 0.0001f);
        expectWithinAbsoluteError(whiteValues[ControlVector::rightHalfDensity], 1.0f / 36.0f, 0.0001f);

        beginTest("center stones contribute to center and occupied densities");

        BoardState centerBoard;
        centerBoard.setCell(4, 4, BoardState::CellState::Black);
        centerBoard.setCell(3, 5, BoardState::CellState::White);

        const auto centerValues = mappingEngine.mapFeatures(FeatureExtractor::extract(centerBoard));

        expectWithinAbsoluteError(centerValues[ControlVector::blackStoneDensity], 1.0f / 81.0f, 0.0001f);
        expectWithinAbsoluteError(centerValues[ControlVector::whiteStoneDensity], 1.0f / 81.0f, 0.0001f);
        expectWithinAbsoluteError(centerValues[ControlVector::occupiedDensity], 2.0f / 81.0f, 0.0001f);
        expectWithinAbsoluteError(centerValues[ControlVector::centerAreaDensity], 2.0f / 9.0f, 0.0001f);
        expectWithinAbsoluteError(centerValues[ControlVector::topHalfDensity], 1.0f / 36.0f, 0.0001f);
        expectWithinAbsoluteError(centerValues[ControlVector::bottomHalfDensity], 0.0f, 0.0001f);
        expectWithinAbsoluteError(centerValues[ControlVector::leftHalfDensity], 0.0f, 0.0001f);
        expectWithinAbsoluteError(centerValues[ControlVector::rightHalfDensity], 1.0f / 36.0f, 0.0001f);
    }
};

static MappingEngineTests mappingEngineTests;
