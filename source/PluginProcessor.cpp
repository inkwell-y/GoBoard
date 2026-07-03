#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace
{
constexpr const char* oscHost = "127.0.0.1";
constexpr int oscPort = 9000;
}

PluginProcessor::PluginProcessor()
    : juce::AudioProcessor(BusesProperties()
                               .withInput("Input", juce::AudioChannelSet::stereo(), true)
                               .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      state(*this, nullptr, "STATE", createParameterLayout())
{
    ensureOscConnected();
    boardStateChanged();
}

void PluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused(sampleRate, samplesPerBlock);
    ensureOscConnected();
}

void PluginProcessor::releaseResources()
{
}

bool PluginProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    const auto mainOutput = layouts.getMainOutputChannelSet();

    if (mainOutput != juce::AudioChannelSet::mono() && mainOutput != juce::AudioChannelSet::stereo())
        return false;

    return mainOutput == layouts.getMainInputChannelSet();
}

void PluginProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    const auto totalNumInputChannels = getTotalNumInputChannels();
    const auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto channel = totalNumInputChannels; channel < totalNumOutputChannels; ++channel)
        buffer.clear(channel, 0, buffer.getNumSamples());

    const juce::ScopedLock lock(pendingMidiLock);
    midiMessages.addEvents(pendingMidiMessages, 0, buffer.getNumSamples(), 0);
    pendingMidiMessages.clear();
}

juce::AudioProcessorEditor* PluginProcessor::createEditor()
{
    return new PluginEditor(*this);
}

bool PluginProcessor::hasEditor() const
{
    return true;
}

const juce::String PluginProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PluginProcessor::acceptsMidi() const
{
    return false;
}

bool PluginProcessor::producesMidi() const
{
    return true;
}

bool PluginProcessor::isMidiEffect() const
{
    return false;
}

double PluginProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PluginProcessor::getNumPrograms()
{
    return 1;
}

int PluginProcessor::getCurrentProgram()
{
    return 0;
}

void PluginProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String PluginProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void PluginProcessor::changeProgramName(int index, const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
}

void PluginProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    if (auto xml = state.copyState().createXml())
        copyXmlToBinary(*xml, destData);
}

void PluginProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    if (auto xmlState = getXmlFromBinary(data, sizeInBytes))
        state.replaceState(juce::ValueTree::fromXml(*xmlState));
}

juce::AudioProcessorValueTreeState& PluginProcessor::getState() noexcept
{
    return state;
}

BoardState& PluginProcessor::getBoardState() noexcept
{
    return boardState;
}

const BoardState& PluginProcessor::getBoardState() const noexcept
{
    return boardState;
}

void PluginProcessor::boardStateChanged()
{
    const auto macroValues = MappingEngine::mapBoardState(boardState);
    queueMacroMidiMessages(macroValues);
    sendMacroOscMessages(macroValues);
}

juce::AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout()
{
    return {};
}

bool PluginProcessor::ensureOscConnected()
{
    if (oscConnected)
        return true;

    oscConnected = oscSender.connect(oscHost, oscPort);
    return oscConnected;
}

void PluginProcessor::queueMacroMidiMessages(const MappingEngine::MacroValues& values)
{
    const std::array<float, 8> macroValues {
        values.blackStoneDensity,
        values.whiteStoneDensity,
        values.occupiedDensity,
        values.centerAreaDensity,
        values.topHalfDensity,
        values.bottomHalfDensity,
        values.leftHalfDensity,
        values.rightHalfDensity
    };

    const juce::ScopedLock lock(pendingMidiLock);

    for (std::size_t index = 0; index < macroValues.size(); ++index)
    {
        const auto midiValue = juce::jlimit(0, 127, juce::roundToInt(macroValues[index] * 127.0f));

        if (midiValue == lastMacroMidiValues[index])
            continue;

        lastMacroMidiValues[index] = midiValue;
        pendingMidiMessages.addEvent(juce::MidiMessage::controllerEvent(1, 20 + static_cast<int>(index), midiValue), 0);
    }
}

void PluginProcessor::sendMacroOscMessages(const MappingEngine::MacroValues& values)
{
    if (! ensureOscConnected())
        return;

    const std::array<float, 8> macroValues {
        values.blackStoneDensity,
        values.whiteStoneDensity,
        values.occupiedDensity,
        values.centerAreaDensity,
        values.topHalfDensity,
        values.bottomHalfDensity,
        values.leftHalfDensity,
        values.rightHalfDensity
    };

    for (std::size_t index = 0; index < macroValues.size(); ++index)
    {
        if (juce::approximatelyEqual(macroValues[index], lastMacroOscValues[index]))
            continue;

        const auto address = juce::String("/goboard/macro/") + juce::String(static_cast<int>(index + 1));

        if (! oscSender.send(address, macroValues[index]))
        {
            oscConnected = false;
            return;
        }

        lastMacroOscValues[index] = macroValues[index];
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}
