#pragma once

#include "PluginProcessor.h"
#include "core/MappingEngine.h"
#include "gui/BoardComponent.h"

#include <array>

class PluginEditor : public juce::AudioProcessorEditor
{
public:
    explicit PluginEditor(PluginProcessor&);
    ~PluginEditor() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void updateMacroLabels();
    void updateTurnLabel();

    PluginProcessor& processorRef;
    BoardComponent boardComponent;
    juce::Label titleLabel;
    juce::Label turnLabel;
    juce::TextButton resetButton { "Reset" };
    std::array<juce::Label, 8> macroLabels;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
};
