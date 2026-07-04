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
        beginTest("reset initializes history with the empty board");

        BoardState boardState;
        GameState gameState(boardState);
        GoRuleEngine ruleEngine(gameState);

        expect(gameState.getBoardHistorySize() == 1);

        beginTest("black plays first and legal moves alternate turns");

        const auto blackMove = ruleEngine.playMove({ 0, 0 });
        expect(blackMove.legal);
        expect(boardState.getCell(0, 0) == BoardState::CellState::Black);
        expect(gameState.getCurrentTurn() == BoardState::CellState::White);
        expect(gameState.getBoardHistorySize() == 2);

        const auto whiteMove = ruleEngine.playMove({ 0, 1 });
        expect(whiteMove.legal);
        expect(boardState.getCell(0, 1) == BoardState::CellState::White);
        expect(gameState.getCurrentTurn() == BoardState::CellState::Black);
        expect(gameState.getBoardHistorySize() == 3);

        beginTest("occupied intersections are illegal and do not change turn");

        const auto occupiedMove = ruleEngine.playMove({ 0, 0 });
        expect(!occupiedMove.legal);
        expect(occupiedMove.reason == MoveFailureReason::OccupiedIntersection);
        expect(gameState.getCurrentTurn() == BoardState::CellState::Black);
        expect(gameState.getBoardHistorySize() == 3);

        beginTest("reset clears the board and restores black to move");

        gameState.reset();
        expect(boardState.countOccupied() == 0);
        expect(gameState.getCurrentTurn() == BoardState::CellState::Black);
        expect(gameState.getBoardHistorySize() == 1);

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

        beginTest("white surrounds one black stone and captures it");

        gameState.reset();
        boardState.setCell(4, 4, BoardState::CellState::Black);
        boardState.setCell(3, 4, BoardState::CellState::White);
        boardState.setCell(4, 3, BoardState::CellState::White);
        boardState.setCell(5, 4, BoardState::CellState::White);
        gameState.advanceTurn();

        const auto whiteCapture = ruleEngine.playMove({ 4, 5 });
        expect(whiteCapture.legal);
        expect(boardState.getCell(4, 4) == BoardState::CellState::Empty);
        expect(boardState.getCell(4, 5) == BoardState::CellState::White);

        beginTest("black captures a connected white group of two stones");

        gameState.reset();
        boardState.setCell(4, 4, BoardState::CellState::White);
        boardState.setCell(4, 5, BoardState::CellState::White);
        boardState.setCell(3, 4, BoardState::CellState::Black);
        boardState.setCell(5, 4, BoardState::CellState::Black);
        boardState.setCell(4, 3, BoardState::CellState::Black);
        boardState.setCell(3, 5, BoardState::CellState::Black);
        boardState.setCell(5, 5, BoardState::CellState::Black);

        const auto blackCapture = ruleEngine.playMove({ 4, 6 });
        expect(blackCapture.legal);
        expect(boardState.getCell(4, 4) == BoardState::CellState::Empty);
        expect(boardState.getCell(4, 5) == BoardState::CellState::Empty);
        expect(boardState.getCell(4, 6) == BoardState::CellState::Black);

        beginTest("capturing one group does not remove unrelated stones");

        gameState.reset();
        boardState.setCell(0, 0, BoardState::CellState::Black);
        boardState.setCell(8, 8, BoardState::CellState::White);
        boardState.setCell(4, 4, BoardState::CellState::White);
        boardState.setCell(4, 5, BoardState::CellState::White);
        boardState.setCell(3, 4, BoardState::CellState::Black);
        boardState.setCell(5, 4, BoardState::CellState::Black);
        boardState.setCell(4, 3, BoardState::CellState::Black);
        boardState.setCell(3, 5, BoardState::CellState::Black);
        boardState.setCell(5, 5, BoardState::CellState::Black);

        const auto isolatedCapture = ruleEngine.playMove({ 4, 6 });
        expect(isolatedCapture.legal);
        expect(boardState.getCell(0, 0) == BoardState::CellState::Black);
        expect(boardState.getCell(8, 8) == BoardState::CellState::White);

        beginTest("captured cells become empty");

        expect(boardState.getCell(4, 4) == BoardState::CellState::Empty);
        expect(boardState.getCell(4, 5) == BoardState::CellState::Empty);

        beginTest("a fully surrounded point is illegal if it captures nothing");

        gameState.reset();
        boardState.setCell(3, 4, BoardState::CellState::White);
        boardState.setCell(4, 3, BoardState::CellState::White);
        boardState.setCell(5, 4, BoardState::CellState::White);
        boardState.setCell(4, 5, BoardState::CellState::White);

        const auto suicideMove = ruleEngine.playMove({ 4, 4 });
        expect(!suicideMove.legal);
        expect(suicideMove.reason == MoveFailureReason::Suicide);
        expect(boardState.getCell(4, 4) == BoardState::CellState::Empty);
        expect(boardState.countOccupied() == 4);

        beginTest("a move that captures first is legal even if the point had no liberties before capture");

        gameState.reset();
        boardState.setCell(3, 4, BoardState::CellState::White);
        boardState.setCell(4, 3, BoardState::CellState::White);
        boardState.setCell(5, 4, BoardState::CellState::White);
        boardState.setCell(4, 5, BoardState::CellState::White);
        boardState.setCell(2, 4, BoardState::CellState::Black);
        boardState.setCell(3, 3, BoardState::CellState::Black);
        boardState.setCell(3, 5, BoardState::CellState::Black);
        boardState.setCell(4, 2, BoardState::CellState::Black);
        boardState.setCell(5, 3, BoardState::CellState::Black);
        boardState.setCell(6, 4, BoardState::CellState::Black);
        boardState.setCell(5, 5, BoardState::CellState::Black);
        boardState.setCell(4, 6, BoardState::CellState::Black);

        const auto captureEscapeMove = ruleEngine.playMove({ 4, 4 });
        expect(captureEscapeMove.legal);
        expect(boardState.getCell(4, 4) == BoardState::CellState::Black);
        expect(boardState.getCell(3, 4) == BoardState::CellState::Empty);
        expect(boardState.getCell(4, 3) == BoardState::CellState::Empty);
        expect(boardState.getCell(5, 4) == BoardState::CellState::Empty);
        expect(boardState.getCell(4, 5) == BoardState::CellState::Empty);
        expect(gameState.getCurrentTurn() == BoardState::CellState::White);

        beginTest("illegal suicide reverts board state and does not switch turn");

        gameState.reset();
        boardState.setCell(3, 4, BoardState::CellState::White);
        boardState.setCell(4, 3, BoardState::CellState::White);
        boardState.setCell(5, 4, BoardState::CellState::White);
        boardState.setCell(4, 5, BoardState::CellState::White);
        const auto occupiedBeforeSuicide = boardState.countOccupied();

        const auto revertedMove = ruleEngine.playMove({ 4, 4 });
        expect(!revertedMove.legal);
        expect(boardState.countOccupied() == occupiedBeforeSuicide);
        expect(boardState.getCell(3, 4) == BoardState::CellState::White);
        expect(boardState.getCell(4, 3) == BoardState::CellState::White);
        expect(boardState.getCell(5, 4) == BoardState::CellState::White);
        expect(boardState.getCell(4, 5) == BoardState::CellState::White);
        expect(gameState.getCurrentTurn() == BoardState::CellState::Black);

        beginTest("a repeated board position is rejected and does not switch turn");

        gameState.reset();
        expect(ruleEngine.playMove({ 8, 8 }).legal);
        expect(ruleEngine.playMove({ 1, 0 }).legal);
        expect(ruleEngine.playMove({ 0, 2 }).legal);
        expect(ruleEngine.playMove({ 0, 1 }).legal);
        expect(ruleEngine.playMove({ 1, 3 }).legal);
        expect(ruleEngine.playMove({ 2, 1 }).legal);
        expect(ruleEngine.playMove({ 2, 2 }).legal);
        expect(ruleEngine.playMove({ 1, 2 }).legal);

        const auto historyBeforeCapture = gameState.getBoardHistorySize();
        const auto blackKoCapture = ruleEngine.playMove({ 1, 1 });
        expect(blackKoCapture.legal);
        expect(gameState.getBoardHistorySize() == historyBeforeCapture + 1);
        expect(boardState.getCell(1, 1) == BoardState::CellState::Black);
        expect(boardState.getCell(1, 2) == BoardState::CellState::Empty);
        expect(gameState.getCurrentTurn() == BoardState::CellState::White);

        const auto historyBeforeRecapture = gameState.getBoardHistorySize();
        const auto whiteKoRecapture = ruleEngine.playMove({ 1, 2 });
        expect(!whiteKoRecapture.legal);
        expect(whiteKoRecapture.reason == MoveFailureReason::RepeatedBoardPosition);
        expect(gameState.getBoardHistorySize() == historyBeforeRecapture);
        expect(boardState.getCell(1, 1) == BoardState::CellState::Black);
        expect(boardState.getCell(1, 2) == BoardState::CellState::Empty);
        expect(gameState.getCurrentTurn() == BoardState::CellState::White);

        beginTest("one pass switches turn and does not end the game");

        gameState.reset();
        const auto firstPass = ruleEngine.passTurn();
        expect(firstPass.legal);
        expect(gameState.getCurrentTurn() == BoardState::CellState::White);
        expect(gameState.getConsecutivePasses() == 1);
        expect(gameState.getGameStatus() == GameStatus::Playing);
        expect(!gameState.isGameOver());

        beginTest("two consecutive passes enter game over");

        const auto secondPass = ruleEngine.passTurn();
        expect(secondPass.legal);
        expect(gameState.getCurrentTurn() == BoardState::CellState::Black);
        expect(gameState.getConsecutivePasses() == 2);
        expect(gameState.getGameStatus() == GameStatus::GameOver);
        expect(gameState.isGameOver());

        beginTest("a normal move after one pass resets consecutive passes");

        gameState.reset();
        expect(ruleEngine.passTurn().legal);
        const auto moveAfterPass = ruleEngine.playMove({ 4, 4 });
        expect(moveAfterPass.legal);
        expect(gameState.getConsecutivePasses() == 0);
        expect(gameState.getCurrentTurn() == BoardState::CellState::Black);
        expect(gameState.getGameStatus() == GameStatus::Playing);

        beginTest("reset exits game over and reinitializes history");

        gameState.reset();
        expect(ruleEngine.passTurn().legal);
        expect(ruleEngine.passTurn().legal);
        expect(gameState.isGameOver());

        gameState.reset();
        expect(!gameState.isGameOver());
        expect(gameState.getGameStatus() == GameStatus::Playing);
        expect(gameState.getCurrentTurn() == BoardState::CellState::Black);
        expect(gameState.getConsecutivePasses() == 0);
        expect(gameState.getBoardHistorySize() == 1);
        expect(boardState.countOccupied() == 0);

        beginTest("empty board with default komi gives white the win");

        gameState.reset();
        expect(ruleEngine.passTurn().legal);
        expect(ruleEngine.passTurn().legal);

        const auto emptyBoardScore = ruleEngine.calculateAreaScore();
        expect(emptyBoardScore.valid);
        expectWithinAbsoluteError(emptyBoardScore.blackScore, 0.0, 0.0001);
        expectWithinAbsoluteError(emptyBoardScore.whiteScore, 7.5, 0.0001);
        expect(emptyBoardScore.winner == ScoreWinner::White);

        beginTest("a one-point territory surrounded by black counts for black");

        gameState.reset();
        boardState.setCell(3, 4, BoardState::CellState::Black);
        boardState.setCell(4, 3, BoardState::CellState::Black);
        boardState.setCell(5, 4, BoardState::CellState::Black);
        boardState.setCell(4, 5, BoardState::CellState::Black);
        expect(ruleEngine.passTurn().legal);
        expect(ruleEngine.passTurn().legal);

        const auto blackTerritoryScore = ruleEngine.calculateAreaScore();
        expect(blackTerritoryScore.valid);
        expectWithinAbsoluteError(blackTerritoryScore.blackScore, 5.0, 0.0001);
        expectWithinAbsoluteError(blackTerritoryScore.whiteScore, 7.5, 0.0001);

        beginTest("a one-point territory surrounded by white counts for white");

        gameState.reset();
        boardState.setCell(3, 4, BoardState::CellState::White);
        boardState.setCell(4, 3, BoardState::CellState::White);
        boardState.setCell(5, 4, BoardState::CellState::White);
        boardState.setCell(4, 5, BoardState::CellState::White);
        expect(ruleEngine.passTurn().legal);
        expect(ruleEngine.passTurn().legal);

        const auto whiteTerritoryScore = ruleEngine.calculateAreaScore();
        expect(whiteTerritoryScore.valid);
        expectWithinAbsoluteError(whiteTerritoryScore.blackScore, 0.0, 0.0001);
        expectWithinAbsoluteError(whiteTerritoryScore.whiteScore, 12.5, 0.0001);

        beginTest("neutral empty regions do not count for either player");

        gameState.reset();
        boardState.setCell(3, 4, BoardState::CellState::Black);
        boardState.setCell(5, 4, BoardState::CellState::Black);
        boardState.setCell(4, 3, BoardState::CellState::White);
        boardState.setCell(4, 5, BoardState::CellState::White);
        expect(ruleEngine.passTurn().legal);
        expect(ruleEngine.passTurn().legal);

        const auto neutralRegionScore = ruleEngine.calculateAreaScore();
        expect(neutralRegionScore.valid);
        expectWithinAbsoluteError(neutralRegionScore.blackScore, 2.0, 0.0001);
        expectWithinAbsoluteError(neutralRegionScore.whiteScore, 9.5, 0.0001);

        beginTest("white score includes configurable komi");

        gameState.reset();
        gameState.setKomi(6.5);
        boardState.setCell(0, 0, BoardState::CellState::White);
        expect(ruleEngine.passTurn().legal);
        expect(ruleEngine.passTurn().legal);

        const auto configurableKomiScore = ruleEngine.calculateAreaScore();
        expect(configurableKomiScore.valid);
        expectWithinAbsoluteError(configurableKomiScore.komi, 6.5, 0.0001);
        expectWithinAbsoluteError(configurableKomiScore.whiteScore, 87.5, 0.0001);
        expect(configurableKomiScore.winner == ScoreWinner::White);
    }
};

static GoRuleEngineTests goRuleEngineTests;
