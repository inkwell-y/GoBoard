#include "GoRuleEngine.h"

#include <queue>
#include <set>

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

std::vector<BoardPosition> GoRuleEngine::neighbors(BoardPosition position) const noexcept
{
    std::vector<BoardPosition> adjacentPositions;
    adjacentPositions.reserve(4);

    for (const auto candidate : {
             BoardPosition { position.row - 1, position.column },
             BoardPosition { position.row + 1, position.column },
             BoardPosition { position.row, position.column - 1 },
             BoardPosition { position.row, position.column + 1 } })
    {
        if (isValidPosition(candidate))
            adjacentPositions.push_back(candidate);
    }

    return adjacentPositions;
}

std::vector<BoardPosition> GoRuleEngine::getGroup(BoardPosition startPosition) const
{
    std::vector<BoardPosition> group;

    if (!isValidPosition(startPosition))
        return group;

    const auto& boardState = gameState.getBoardState();
    const auto targetColour = boardState.getCell(startPosition.row, startPosition.column);

    if (targetColour == BoardState::CellState::Empty)
        return group;

    std::queue<BoardPosition> positionsToVisit;
    std::set<BoardPosition> visited;

    positionsToVisit.push(startPosition);
    visited.insert(startPosition);

    while (!positionsToVisit.empty())
    {
        const auto current = positionsToVisit.front();
        positionsToVisit.pop();
        group.push_back(current);

        for (const auto neighbor : neighbors(current))
        {
            if (visited.find(neighbor) != visited.end())
                continue;

            if (boardState.getCell(neighbor.row, neighbor.column) != targetColour)
                continue;

            visited.insert(neighbor);
            positionsToVisit.push(neighbor);
        }
    }

    return group;
}

int GoRuleEngine::countLiberties(const std::vector<BoardPosition>& group) const noexcept
{
    std::set<BoardPosition> liberties;
    const auto& boardState = gameState.getBoardState();

    for (const auto position : group)
    {
        for (const auto neighbor : neighbors(position))
        {
            if (boardState.getCell(neighbor.row, neighbor.column) == BoardState::CellState::Empty)
                liberties.insert(neighbor);
        }
    }

    return static_cast<int>(liberties.size());
}

MoveResult GoRuleEngine::playMove(BoardPosition position) noexcept
{
    if (!isValidPosition(position))
        return { false, MoveFailureReason::OutOfBounds };

    auto& boardState = gameState.getBoardState();
    const auto currentPlayer = gameState.getCurrentTurn();

    if (boardState.getCell(position.row, position.column) != BoardState::CellState::Empty)
        return { false, MoveFailureReason::OccupiedIntersection };

    boardState.setCell(position.row, position.column, currentPlayer);

    std::set<BoardPosition> processedOpponentStones;

    for (const auto neighbor : neighbors(position))
    {
        if (boardState.getCell(neighbor.row, neighbor.column) != oppositeColor(currentPlayer))
            continue;

        if (processedOpponentStones.find(neighbor) != processedOpponentStones.end())
            continue;

        const auto opponentGroup = getGroup(neighbor);
        processedOpponentStones.insert(opponentGroup.begin(), opponentGroup.end());

        if (countLiberties(opponentGroup) == 0)
            removeGroup(opponentGroup);
    }

    gameState.advanceTurn();
    return { true, MoveFailureReason::None };
}

bool GoRuleEngine::isValidPosition(BoardPosition position) noexcept
{
    return position.row >= 0
        && position.row < BoardState::boardSize
        && position.column >= 0
        && position.column < BoardState::boardSize;
}

BoardState::CellState GoRuleEngine::oppositeColor(BoardState::CellState color) noexcept
{
    if (color == BoardState::CellState::Black)
        return BoardState::CellState::White;

    if (color == BoardState::CellState::White)
        return BoardState::CellState::Black;

    return BoardState::CellState::Empty;
}

void GoRuleEngine::removeGroup(const std::vector<BoardPosition>& group) noexcept
{
    auto& boardState = gameState.getBoardState();

    for (const auto position : group)
        boardState.setCell(position.row, position.column, BoardState::CellState::Empty);
}
