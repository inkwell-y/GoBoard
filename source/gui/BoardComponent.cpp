#include "BoardComponent.h"

#include <optional>

namespace
{
constexpr int starPointIndices[] { 2, 4, 6 };

static const juce::Colour backgroundColour { 16, 19, 24 };
static const juce::Colour boardFillColour { 193, 154, 107 };
static const juce::Colour boardOutlineColour { 88, 61, 36 };
static const juce::Colour gridColour { 74, 49, 27 };
static const juce::Colour starPointColour { 46, 30, 18 };
static const juce::Colour blackStoneColour { 24, 27, 31 };
static const juce::Colour whiteStoneColour { 242, 238, 231 };
static const juce::Colour whiteStoneOutlineColour { 168, 160, 149 };
}

BoardComponent::BoardComponent(BoardState& state)
    : boardState(state)
{
    setOpaque(true);
}

BoardState::CellState BoardComponent::getCurrentTurn() const noexcept
{
    return currentTurn;
}

juce::String BoardComponent::getCurrentTurnText() const
{
    return currentTurn == BoardState::CellState::Black ? "Black" : "White";
}

void BoardComponent::resetGame()
{
    boardState.clear();
    currentTurn = BoardState::CellState::Black;
    repaint();

    if (onGameStateChanged)
        onGameStateChanged();
}

void BoardComponent::paint(juce::Graphics& g)
{
    g.fillAll(backgroundColour);

    const auto geometry = getBoardGeometry();
    drawBoard(g, geometry);
    drawGrid(g, geometry);
    drawStarPoints(g, geometry);
    drawStones(g, geometry);
}

void BoardComponent::mouseDown(const juce::MouseEvent& event)
{
    if (!event.mods.isLeftButtonDown())
        return;

    const auto cell = getCellAtPosition(event.position);

    if (!cell.has_value())
        return;

    const auto row = cell->x;
    const auto column = cell->y;

    if (boardState.getCell(row, column) != BoardState::CellState::Empty)
        return;

    boardState.setCell(row, column, currentTurn);
    advanceTurn();
    repaint();

    if (onGameStateChanged)
        onGameStateChanged();
}

BoardComponent::BoardGeometry BoardComponent::getBoardGeometry() const noexcept
{
    BoardGeometry geometry;

    const auto available = getLocalBounds().toFloat().reduced(24.0f);
    const auto side = std::min(available.getWidth(), available.getHeight());
    const auto square = juce::Rectangle<float>(side, side).withCentre(available.getCentre());

    geometry.cellSpacing = side / static_cast<float>(BoardState::boardSize - 1);
    geometry.stoneRadius = geometry.cellSpacing * 0.42f;
    geometry.boardBounds = square;

    return geometry;
}

juce::Point<float> BoardComponent::getIntersectionPosition(int row, int column, const BoardGeometry& geometry) const noexcept
{
    return {
        geometry.boardBounds.getX() + (geometry.cellSpacing * static_cast<float>(column)),
        geometry.boardBounds.getY() + (geometry.cellSpacing * static_cast<float>(row))
    };
}

std::optional<juce::Point<int>> BoardComponent::getCellAtPosition(juce::Point<float> position) const noexcept
{
    const auto geometry = getBoardGeometry();
    const auto tolerance = geometry.cellSpacing * 0.5f;

    if (!geometry.boardBounds.expanded(tolerance).contains(position))
        return std::nullopt;

    const auto column = juce::jlimit(0, BoardState::boardSize - 1,
                                     juce::roundToInt((position.x - geometry.boardBounds.getX()) / geometry.cellSpacing));
    const auto row = juce::jlimit(0, BoardState::boardSize - 1,
                                  juce::roundToInt((position.y - geometry.boardBounds.getY()) / geometry.cellSpacing));

    const auto intersection = getIntersectionPosition(row, column, geometry);

    if (position.getDistanceFrom(intersection) > tolerance)
        return std::nullopt;

    return juce::Point<int>(row, column);
}

void BoardComponent::drawBoard(juce::Graphics& g, const BoardGeometry& geometry) const
{
    const auto outerBounds = geometry.boardBounds.expanded(geometry.cellSpacing * 0.55f);

    g.setColour(boardFillColour);
    g.fillRoundedRectangle(outerBounds, 12.0f);

    g.setColour(boardOutlineColour);
    g.drawRoundedRectangle(outerBounds, 12.0f, 2.0f);
}

void BoardComponent::drawGrid(juce::Graphics& g, const BoardGeometry& geometry) const
{
    g.setColour(gridColour);

    for (int index = 0; index < BoardState::boardSize; ++index)
    {
        const auto offset = geometry.cellSpacing * static_cast<float>(index);

        g.drawLine(geometry.boardBounds.getX(),
                   geometry.boardBounds.getY() + offset,
                   geometry.boardBounds.getRight(),
                   geometry.boardBounds.getY() + offset,
                   1.4f);

        g.drawLine(geometry.boardBounds.getX() + offset,
                   geometry.boardBounds.getY(),
                   geometry.boardBounds.getX() + offset,
                   geometry.boardBounds.getBottom(),
                   1.4f);
    }
}

void BoardComponent::drawStarPoints(juce::Graphics& g, const BoardGeometry& geometry) const
{
    g.setColour(starPointColour);

    for (const auto row : starPointIndices)
    {
        for (const auto column : starPointIndices)
        {
            const auto centre = getIntersectionPosition(row, column, geometry);
            g.fillEllipse(juce::Rectangle<float>(7.0f, 7.0f).withCentre(centre));
        }
    }
}

void BoardComponent::drawStones(juce::Graphics& g, const BoardGeometry& geometry) const
{
    for (int row = 0; row < BoardState::boardSize; ++row)
    {
        for (int column = 0; column < BoardState::boardSize; ++column)
        {
            const auto cell = boardState.getCell(row, column);

            if (cell == BoardState::CellState::Empty)
                continue;

            const auto centre = getIntersectionPosition(row, column, geometry);
            const auto stoneBounds = juce::Rectangle<float>(geometry.stoneRadius * 2.0f, geometry.stoneRadius * 2.0f).withCentre(centre);

            if (cell == BoardState::CellState::Black)
            {
                g.setColour(blackStoneColour);
                g.fillEllipse(stoneBounds);
            }
            else
            {
                g.setColour(whiteStoneColour);
                g.fillEllipse(stoneBounds);
                g.setColour(whiteStoneOutlineColour);
                g.drawEllipse(stoneBounds, 1.5f);
            }
        }
    }
}

void BoardComponent::advanceTurn() noexcept
{
    currentTurn = currentTurn == BoardState::CellState::Black
                    ? BoardState::CellState::White
                    : BoardState::CellState::Black;
}
