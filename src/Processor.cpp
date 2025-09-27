#include "Processor.hpp"

bool ProcessorResult::operator==(const ProcessorResult &other) const
{
    return timestamp_first_command == other.timestamp_first_command &&
            commandsResult == other.commandsResult;
}

std::string ProcessorResult::toString() const
{
    std::string result = "bulk: ";
    int commandsCount = commandsResult.size();
    for (int i = 0; i< commandsCount; ++i) {
        result += commandsResult.at(i);
        if (i != commandsCount -1) {
            result +=  ", ";
        }
    }
    return result;
}

Processor::Processor(size_t n,
                     std::unique_ptr<std::vector<std::unique_ptr<IOutputGenerator> > > output,
                     ITimeProvider* timeProvider)
{
    m_bulkSize = n;
    m_outputGenerators = std::move(output);
    m_timeProvider = timeProvider;
}

void Processor::addCommand(std::unique_ptr<ICommand> command)
{
    if (m_processResult.commandsResult.size() == 0) {
        m_processResult.timestamp_first_command = m_timeProvider->getCurrentTimestamp();
    }

    if (command->isEof()) {
        if (!m_isDynamicBlock) {
            endBlock();
            return;
        } else {
            clearResults();
            return;
        }
    }

    std::string commandResult = command->execute();

    if (commandResult == "{" && !m_isDynamicBlock) {
        m_isDynamicBlock = true;
        endBlock();
        return;
    } else if (commandResult == "{" && m_isDynamicBlock) {
        ++m_innerBlock;
        return;
    } else if (commandResult == "}" && m_isDynamicBlock && !isInnerBlock()) {
        m_isDynamicBlock = false;
        endBlock();
        return;
    } else if (commandResult == "}" && isInnerBlock()) {
       --m_innerBlock;
       return;
   }

    m_processResult.commandsResult.push_back(commandResult);

    if (m_processResult.commandsResult.size() == m_bulkSize &&
        !m_isDynamicBlock) {
        endBlock();
    }
}

bool Processor::isInnerBlock()
{
    return m_innerBlock != 0;
}

void Processor::endBlock()
{
    if (m_processResult.commandsResult.size() == 0) {
        return;
    }

    for (const auto& outputGenerator : *m_outputGenerators) {
        outputGenerator->generateReport(m_processResult);
    }

    clearResults();
}

void Processor::clearResults()
{
    m_processResult.commandsResult.clear();
}
