#pragma once

#include "BoardState.h"

#include <unordered_set>
#include <vector>

struct BoardPosition
{
    int row = 0;
    int column = 0;

    bool operator==(const BoardPosition& other) const noexcept = default;
    bool operator<(const BoardPosition& other) const noexcept
    {
        if (row != other.row)
            return row < other.row;

        return column < other.column;
    }
};

enum class MoveFailureReason
{
    None,
    OutOfBounds,
    OccupiedIntersection,
    Suicide,
    RepeatedBoardPosition
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
    std::size_t getBoardHistorySize() const noexcept;
    bool hasSeenCurrentBoardPosition() const noexcept;
    void recordCurrentBoardPosition() noexcept;
    void reset() noexcept;
    void advanceTurn() noexcept;

private:
    std::size_t computeCurrentBoardHash() const noexcept;
    void clearHistoryAndRecordCurrentBoard() noexcept;

    BoardState& boardState;
    BoardState::CellState currentTurn = BoardState::CellState::Black;
    std::unordered_set<std::size_t> boardHistory;
};

class GoRuleEngine
{
public:
    explicit GoRuleEngine(GameState&);

    std::vector<BoardPosition> neighbors(BoardPosition) const noexcept;
    std::vector<BoardPosition> getGroup(BoardPosition) const;
    int countLiberties(const std::vector<BoardPosition>& group) const noexcept;
    MoveResult playMove(BoardPosition) noexcept;

private:
    static bool isValidPosition(BoardPosition) noexcept;
    static BoardState::CellState oppositeColor(BoardState::CellState) noexcept;
    void removeGroup(const std::vector<BoardPosition>& group) noexcept;

    GameState& gameState;
};
