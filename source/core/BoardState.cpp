#include "BoardState.h"

#include <algorithm>

BoardState::BoardState()
{
    clear();
}

void BoardState::clear() noexcept
{
    cells.fill(CellState::Empty);
}

BoardState::CellState BoardState::getCell(int row, int column) const noexcept
{
    if (!isValidPosition(row, column))
        return CellState::Empty;

    return cells[static_cast<std::size_t>(indexFor(row, column))];
}

void BoardState::setCell(int row, int column, CellState state) noexcept
{
    if (!isValidPosition(row, column))
        return;

    cells[static_cast<std::size_t>(indexFor(row, column))] = state;
}

void BoardState::cycleCell(int row, int column) noexcept
{
    switch (getCell(row, column))
    {
        case CellState::Empty:
            setCell(row, column, CellState::Black);
            break;

        case CellState::Black:
            setCell(row, column, CellState::White);
            break;

        case CellState::White:
            setCell(row, column, CellState::Empty);
            break;
    }
}

int BoardState::countBlack() const noexcept
{
    return static_cast<int>(std::count(cells.begin(), cells.end(), CellState::Black));
}

int BoardState::countWhite() const noexcept
{
    return static_cast<int>(std::count(cells.begin(), cells.end(), CellState::White));
}

int BoardState::countOccupied() const noexcept
{
    return countBlack() + countWhite();
}

bool BoardState::isValidPosition(int row, int column) noexcept
{
    return row >= 0 && row < boardSize && column >= 0 && column < boardSize;
}

int BoardState::indexFor(int row, int column) noexcept
{
    return (row * boardSize) + column;
}
