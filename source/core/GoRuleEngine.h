#pragma once

#include "BoardState.h"

struct BoardPosition
{
    int row = 0;
    int column = 0;
};

enum class MoveFailureReason
{
    None,
    OutOfBounds,
    OccupiedIntersection
};

struct MoveResult
{
    bool legal = false;
    MoveFailureReason reason = MoveFailureReason::None;
};

class GameState
{
public:
    explicit GameState(BoardState&);

    BoardState& getBoardState() noexcept;
    const BoardState& getBoardState() const noexcept;
    BoardState::CellState getCurrentTurn() const noexcept;
    void reset() noexcept;
    void advanceTurn() noexcept;

private:
    BoardState& boardState;
    BoardState::CellState currentTurn = BoardState::CellState::Black;
};

class GoRuleEngine
{
public:
    explicit GoRuleEngine(GameState&);

    MoveResult playMove(BoardPosition) noexcept;

private:
    GameState& gameState;
};
