#pragma once

#include "core/BoardState.h"
#include "core/MappingEngine.h"

#include <juce_audio_processors/juce_audio_processors.h>
#include <array>

class PluginProcessor : public juce::AudioProcessor
{
public:
    PluginProcessor();
    ~PluginProcessor() override = default;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState& getState() noexcept;
    BoardState& getBoardState() noexcept;
    const BoardState& getBoardState() const noexcept;
    void boardStateChanged();

private:
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void queueMacroMidiMessages(const MappingEngine::MacroValues&);

    juce::AudioProcessorValueTreeState state;
    BoardState boardState;
    juce::CriticalSection pendingMidiLock;
    juce::MidiBuffer pendingMidiMessages;
    std::array<int, 8> lastMacroMidiValues { -1, -1, -1, -1, -1, -1, -1, -1 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginProcessor)
};
