#include <juce_core/juce_core.h>

#include <iostream>
#include <vector>

class ConsoleLogger final : public juce::Logger
{
public:
    void logMessage(const juce::String& message) override
    {
        std::cout << message << std::endl;
    }
};

class ConsoleUnitTestRunner final : public juce::UnitTestRunner
{
public:
    void logMessage(const juce::String& message) override
    {
        juce::Logger::writeToLog(message);
    }
};

int main()
{
    ConsoleLogger logger;
    juce::Logger::setCurrentLogger(&logger);

    const juce::ScopeGuard onExit { [&]
    {
        juce::Logger::setCurrentLogger(nullptr);
    } };

    ConsoleUnitTestRunner runner;
    runner.setAssertOnFailure(false);
    runner.runAllTests();

    for (int index = 0; index < runner.getNumResults(); ++index)
    {
        if (const auto* result = runner.getResult(index); result != nullptr && result->failures > 0)
            return 1;
    }

    return 0;
}
