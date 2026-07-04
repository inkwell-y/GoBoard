#pragma once

#include "core/GoRuleEngine.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include <functional>
#include <optional>

class BoardComponent : public juce::Component
{
public:
    BoardComponent(GameState&, GoRuleEngine&);

    BoardState::CellState getCurrentTurn() const noexcept;
    AreaScore getAreaScore() const;
    bool isGameOver() const noexcept;
    juce::String getCurrentTurnText() const;
    juce::String getStatusText() const;
    void passTurn();
    void resetGame();

    void paint(juce::Graphics&) override;
    void mouseDown(const juce::MouseEvent&) override;

    std::function<void()> onGameStateChanged;

private:
    struct BoardGeometry
    {
        juce::Rectangle<float> boardBounds;
        float cellSpacing = 0.0f;
        float stoneRadius = 0.0f;
    };

    BoardGeometry getBoardGeometry() const noexcept;
    juce::Point<float> getIntersectionPosition(int row, int column, const BoardGeometry&) const noexcept;
    std::optional<juce::Point<int>> getCellAtPosition(juce::Point<float>) const noexcept;

    void drawBoard(juce::Graphics&, const BoardGeometry&) const;
    void drawGrid(juce::Graphics&, const BoardGeometry&) const;
    void drawStarPoints(juce::Graphics&, const BoardGeometry&) const;
    void drawStones(juce::Graphics&, const BoardGeometry&) const;

    GameState& gameState;
    GoRuleEngine& goRuleEngine;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BoardComponent)
};
