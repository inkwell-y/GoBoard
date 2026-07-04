#include <juce_core/juce_core.h>

#include "core/BoardState.h"
#include "core/GoRuleEngine.h"

#include <algorithm>

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

        beginTest("single center stone has 4 liberties");

        boardState.clear();
        boardState.setCell(4, 4, BoardState::CellState::Black);
        expect(ruleEngine.countLiberties(ruleEngine.getGroup({ 4, 4 })) == 4);

        beginTest("corner stone has 2 liberties");

        boardState.clear();
        boardState.setCell(0, 0, BoardState::CellState::Black);
        expect(ruleEngine.countLiberties(ruleEngine.getGroup({ 0, 0 })) == 2);

        beginTest("edge stone has 3 liberties");

        boardState.clear();
        boardState.setCell(0, 4, BoardState::CellState::Black);
        expect(ruleEngine.countLiberties(ruleEngine.getGroup({ 0, 4 })) == 3);

        beginTest("connected two-stone group shares liberties correctly");

        boardState.clear();
        boardState.setCell(4, 4, BoardState::CellState::Black);
        boardState.setCell(4, 5, BoardState::CellState::Black);

        const auto connectedGroup = ruleEngine.getGroup({ 4, 4 });
        expect(connectedGroup.size() == 2);
        expect(ruleEngine.countLiberties(connectedGroup) == 6);

        beginTest("diagonal stones are not connected");

        boardState.clear();
        boardState.setCell(4, 4, BoardState::CellState::Black);
        boardState.setCell(5, 5, BoardState::CellState::Black);

        const auto diagonalGroup = ruleEngine.getGroup({ 4, 4 });
        expect(diagonalGroup.size() == 1);
        expect(std::find(diagonalGroup.begin(), diagonalGroup.end(), BoardPosition { 4, 4 }) != diagonalGroup.end());
        expect(std::find(diagonalGroup.begin(), diagonalGroup.end(), BoardPosition { 5, 5 }) == diagonalGroup.end());

        beginTest("neighbors include only orthogonal positions");

        const auto centerNeighbors = ruleEngine.neighbors({ 4, 4 });
        expect(centerNeighbors.size() == 4);
        expect(std::find(centerNeighbors.begin(), centerNeighbors.end(), BoardPosition { 3, 4 }) != centerNeighbors.end());
        expect(std::find(centerNeighbors.begin(), centerNeighbors.end(), BoardPosition { 5, 4 }) != centerNeighbors.end());
        expect(std::find(centerNeighbors.begin(), centerNeighbors.end(), BoardPosition { 4, 3 }) != centerNeighbors.end());
        expect(std::find(centerNeighbors.begin(), centerNeighbors.end(), BoardPosition { 4, 5 }) != centerNeighbors.end());
        expect(std::find(centerNeighbors.begin(), centerNeighbors.end(), BoardPosition { 3, 3 }) == centerNeighbors.end());
    }
};

static GoRuleEngineTests goRuleEngineTests;
