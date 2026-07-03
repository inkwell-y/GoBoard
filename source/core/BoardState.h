#pragma once

#include <array>

class BoardState
{
public:
    enum class CellState
    {
        Empty,
        Black,
        White
    };

    static constexpr int boardSize = 9;
    static constexpr int numCells = boardSize * boardSize;

    BoardState();

    void clear() noexcept;
    CellState getCell(int row, int column) const noexcept;
    void setCell(int row, int column, CellState state) noexcept;
    void cycleCell(int row, int column) noexcept;

    int countBlack() const noexcept;
    int countWhite() const noexcept;
    int countOccupied() const noexcept;

private:
    using Storage = std::array<CellState, numCells>;

    static bool isValidPosition(int row, int column) noexcept;
    static int indexFor(int row, int column) noexcept;

    Storage cells {};
};
