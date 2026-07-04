#include "GoRuleEngine.h"

namespace
{
bool isValidPosition(BoardPosition position) noexcept
{
    return position.row >= 0
        && position.row < BoardState::boardSize
        && position.column >= 0
        && position.column < BoardState::boardSize;
}
}

GameState::GameState(BoardState& state)
    : boardState(state)
{
}

BoardState& GameState::getBoardState() noexcept
{
    return boardState;
}

const BoardState& GameState::getBoardState() const noexcept
{
    return boardState;
}

BoardState::CellState GameState::getCurrentTurn() const noexcept
{
    return currentTurn;
}

void GameState::reset() noexcept
{
    boardState.clear();
    currentTurn = BoardState::CellState::Black;
}

void GameState::advanceTurn() noexcept
{
    currentTurn = currentTurn == BoardState::CellState::Black
                    ? BoardState::CellState::White
                    : BoardState::CellState::Black;
}

GoRuleEngine::GoRuleEngine(GameState& state)
    : gameState(state)
{
}

MoveResult GoRuleEngine::playMove(BoardPosition position) noexcept
{
    if (!isValidPosition(position))
        return { false, MoveFailureReason::OutOfBounds };

    auto& boardState = gameState.getBoardState();

    if (boardState.getCell(position.row, position.column) != BoardState::CellState::Empty)
        return { false, MoveFailureReason::OccupiedIntersection };

    boardState.setCell(position.row, position.column, gameState.getCurrentTurn());
    gameState.advanceTurn();
    return { true, MoveFailureReason::None };
}
