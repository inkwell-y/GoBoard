#include <juce_core/juce_core.h>

#include "core/BoardState.h"
#include "core/GoRuleEngine.h"

class GoRuleEngineTests final : public juce::UnitTest
{
public:
    GoRuleEngineTests()
        : juce::UnitTest("GoRuleEngine", "GoBoardController")
    {
    }

    void runTest() override
    {
        beginTest("black plays first and legal moves alternate turns");

        BoardState boardState;
        GameState gameState(boardState);
        GoRuleEngine ruleEngine(gameState);

        const auto blackMove = ruleEngine.playMove({ 0, 0 });
        expect(blackMove.legal);
        expect(boardState.getCell(0, 0) == BoardState::CellState::Black);
        expect(gameState.getCurrentTurn() == BoardState::CellState::White);

        const auto whiteMove = ruleEngine.playMove({ 0, 1 });
        expect(whiteMove.legal);
        expect(boardState.getCell(0, 1) == BoardState::CellState::White);
        expect(gameState.getCurrentTurn() == BoardState::CellState::Black);

        beginTest("occupied intersections are illegal and do not change turn");

        const auto occupiedMove = ruleEngine.playMove({ 0, 0 });
        expect(!occupiedMove.legal);
        expect(occupiedMove.reason == MoveFailureReason::OccupiedIntersection);
        expect(gameState.getCurrentTurn() == BoardState::CellState::Black);

        beginTest("reset clears the board and restores black to move");

        gameState.reset();
        expect(boardState.countOccupied() == 0);
        expect(gameState.getCurrentTurn() == BoardState::CellState::Black);
    }
};

static GoRuleEngineTests goRuleEngineTests;
