#include "PluginEditor.h"

namespace
{
const std::array<const char*, 8> macroNames {
    "Macro 1: Black density",
    "Macro 2: White density",
    "Macro 3: Occupied density",
    "Macro 4: Center area density",
    "Macro 5: Top half density",
    "Macro 6: Bottom half density",
    "Macro 7: Left half density",
    "Macro 8: Right half density"
};
}

PluginEditor::PluginEditor(PluginProcessor& pluginProcessor)
    : juce::AudioProcessorEditor(&pluginProcessor),
      processorRef(pluginProcessor),
      boardComponent(processorRef.getGameState(), processorRef.getGoRuleEngine())
{
    titleLabel.setText("Go Board Controller", juce::dontSendNotification);
    titleLabel.setJustificationType(juce::Justification::centred);
    titleLabel.setFont(juce::FontOptions(24.0f, juce::Font::bold));
    addAndMakeVisible(titleLabel);

    turnLabel.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(turnLabel);

    oscStatusLabel.setText(processorRef.getOscStatusText(), juce::dontSendNotification);
    oscStatusLabel.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(oscStatusLabel);

    passButton.onClick = [this]
    {
        boardComponent.passTurn();
    };
    addAndMakeVisible(passButton);

    resetButton.onClick = [this]
    {
        boardComponent.resetGame();
    };
    addAndMakeVisible(resetButton);

    boardComponent.onGameStateChanged = [this]
    {
        processorRef.boardStateChanged();
        updateTurnLabel();
        updateMacroLabels();
    };

    addAndMakeVisible(boardComponent);

    for (auto& label : macroLabels)
    {
        label.setJustificationType(juce::Justification::centredLeft);
        addAndMakeVisible(label);
    }

    updateTurnLabel();
    updateMacroLabels();

    setResizable(true, false);
    setSize(980, 760);
}

void PluginEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour::fromRGB(18, 22, 28));

    auto bounds = getLocalBounds().reduced(24);
    g.setColour(juce::Colour::fromRGB(50, 61, 72));
    g.fillRoundedRectangle(bounds.toFloat(), 18.0f);

    g.setColour(juce::Colour::fromRGB(122, 183, 127));
    g.drawRoundedRectangle(bounds.toFloat(), 18.0f, 2.0f);
}

void PluginEditor::resized()
{
    auto bounds = getLocalBounds().reduced(48);
    titleLabel.setBounds(bounds.removeFromTop(56));
    bounds.removeFromTop(12);
    auto controls = bounds.removeFromTop(32);
    turnLabel.setBounds(controls.removeFromLeft(controls.getWidth() - 228));
    passButton.setBounds(controls.removeFromRight(100));
    controls.removeFromRight(8);
    resetButton.setBounds(controls.removeFromRight(100));
    bounds.removeFromTop(8);
    oscStatusLabel.setBounds(bounds.removeFromTop(24));
    bounds.removeFromTop(20);

    auto macroArea = bounds.removeFromRight(260);
    bounds.removeFromRight(20);
    boardComponent.setBounds(bounds);

    constexpr int rowHeight = 34;

    for (auto& label : macroLabels)
    {
        label.setBounds(macroArea.removeFromTop(rowHeight));
        macroArea.removeFromTop(8);
    }
}

void PluginEditor::updateTurnLabel()
{
    turnLabel.setText(boardComponent.getStatusText(), juce::dontSendNotification);
    passButton.setEnabled(!boardComponent.isGameOver());
}

void PluginEditor::updateMacroLabels()
{
    const auto& macroValues = processorRef.getCurrentControlVector();

    for (std::size_t index = 0; index < macroLabels.size(); ++index)
    {
        macroLabels[index].setText(juce::String(macroNames[index]) + ": " + juce::String(macroValues[index], 3),
                                   juce::dontSendNotification);
    }
}
