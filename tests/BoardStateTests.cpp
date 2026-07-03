#include <juce_core/juce_core.h>

#include "core/BoardState.h"

class BoardStateTests final : public juce::UnitTest
{
public:
    BoardStateTests()
        : juce::UnitTest("BoardState", "GoBoardController")
    {
    }

    void runTest() override
    {
        beginTest("clear, setCell, and counts");

        BoardState board;
        board.setCell(0, 0, BoardState::CellState::Black);
        board.setCell(4, 4, BoardState::CellState::White);
        board.setCell(8, 8, BoardState::CellState::Black);

        expectEquals(board.countBlack(), 2);
        expectEquals(board.countWhite(), 1);
        expectEquals(board.countOccupied(), 3);
        expect(board.getCell(4, 4) == BoardState::CellState::White);

        board.clear();

        expectEquals(board.countBlack(), 0);
        expectEquals(board.countWhite(), 0);
        expectEquals(board.countOccupied(), 0);
        expect(board.getCell(0, 0) == BoardState::CellState::Empty);

        beginTest("cycleCell advances through empty, black, white");

        board.cycleCell(2, 3);
        expect(board.getCell(2, 3) == BoardState::CellState::Black);

        board.cycleCell(2, 3);
        expect(board.getCell(2, 3) == BoardState::CellState::White);

        board.cycleCell(2, 3);
        expect(board.getCell(2, 3) == BoardState::CellState::Empty);

        beginTest("out-of-range access is ignored safely");

        board.setCell(-1, 0, BoardState::CellState::Black);
        board.setCell(0, 9, BoardState::CellState::White);
        board.cycleCell(99, 99);

        expectEquals(board.countOccupied(), 0);
        expect(board.getCell(-1, -1) == BoardState::CellState::Empty);
    }
};

static BoardStateTests boardStateTests;
