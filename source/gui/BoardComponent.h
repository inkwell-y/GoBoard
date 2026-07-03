#pragma once

#include "core/BoardState.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include <optional>

class BoardComponent : public juce::Component
{
public:
    explicit BoardComponent(BoardState&);

    void paint(juce::Graphics&) override;
    void mouseDown(const juce::MouseEvent&) override;

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

    BoardState& boardState;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BoardComponent)
};
