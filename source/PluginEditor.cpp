#include "PluginEditor.h"

PluginEditor::PluginEditor(PluginProcessor& processor)
    : juce::AudioProcessorEditor(&processor), processorRef(processor)
{
    juce::ignoreUnused(processorRef);

    titleLabel.setText("Go Board Controller", juce::dontSendNotification);
    titleLabel.setJustificationType(juce::Justification::centred);
    titleLabel.setFont(juce::Font(24.0f, juce::Font::bold));
    addAndMakeVisible(titleLabel);

    statusLabel.setText("Skeleton build only. Go board logic, OSC, MIDI, AI, ONNX, FPGA, and synth DSP are not implemented yet.",
                        juce::dontSendNotification);
    statusLabel.setJustificationType(juce::Justification::centred);
    statusLabel.setMinimumHorizontalScale(0.8f);
    addAndMakeVisible(statusLabel);

    setResizable(true, false);
    setSize(640, 360);
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
    bounds.removeFromTop(16);
    statusLabel.setBounds(bounds.removeFromTop(96));
}
