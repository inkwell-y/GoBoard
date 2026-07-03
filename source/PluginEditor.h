#pragma once

#include "PluginProcessor.h"
#include "gui/BoardComponent.h"

class PluginEditor : public juce::AudioProcessorEditor
{
public:
    explicit PluginEditor(PluginProcessor&);
    ~PluginEditor() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void updateTurnLabel();

    PluginProcessor& processorRef;
    BoardComponent boardComponent;
    juce::Label titleLabel;
    juce::Label turnLabel;
    juce::TextButton resetButton { "Reset" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
};
