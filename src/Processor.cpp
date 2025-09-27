#include "Processor.hpp"

bool ProcessorResult::operator==(const ProcessorResult &other) const
{
    return timestamp_first_command == other.timestamp_first_command &&
           commands == other.commands;
}

Processor::Processor(size_t n,
                     const std::vector<IOutputGenerator*>& output,
                     ITimeProvider* timeProvider)
{
    m_bulkSize = n;
    m_outputGenerators = output;
    m_timeProvider = timeProvider;
}

void Processor::addCommand(const std::string &command)
{
    if (m_processResult.commands.size() == 0) {
        m_processResult.timestamp_first_command = m_timeProvider->getCurrentTimestamp();
    }

    m_processResult.commands.push_back(command);

    if (m_processResult.commands.size() == m_bulkSize) {

        for (const auto outputGenerator : m_outputGenerators) {
            outputGenerator->generateReport(m_processResult);
        }

        clearResults();
    }
}

void Processor::clearResults()
{
    m_processResult.commands.clear();
}
