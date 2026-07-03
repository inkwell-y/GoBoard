#include "PluginEditor.h"

PluginEditor::PluginEditor(PluginProcessor& pluginProcessor)
    : juce::AudioProcessorEditor(&pluginProcessor),
      processorRef(pluginProcessor),
      boardComponent(processorRef.getBoardState())
{
    titleLabel.setText("Go Board Controller", juce::dontSendNotification);
    titleLabel.setJustificationType(juce::Justification::centred);
    titleLabel.setFont(juce::FontOptions(24.0f, juce::Font::bold));
    addAndMakeVisible(titleLabel);

    interactionLabel.setText("Left click cycles Empty -> Black -> White. Right click clears a cell.",
                             juce::dontSendNotification);
    interactionLabel.setJustificationType(juce::Justification::centred);
    interactionLabel.setMinimumHorizontalScale(0.8f);
    addAndMakeVisible(interactionLabel);

    addAndMakeVisible(boardComponent);

    setResizable(true, false);
    setSize(720, 720);
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
    interactionLabel.setBounds(bounds.removeFromTop(32));
    bounds.removeFromTop(20);
    boardComponent.setBounds(bounds);
}
