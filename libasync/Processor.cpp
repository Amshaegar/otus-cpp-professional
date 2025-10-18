#include "Processor.hpp"

#include <atomic>
#include <mutex>
#include <shared_mutex>

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

std::atomic<bool> stopLogThread = false;
std::atomic<bool> stopFile1Thread = false;
std::atomic<bool> stopFile2Thread = false;

std::atomic<bool> haveLogResult = false;
std::atomic<bool> haveFile1Result = false;
std::atomic<bool> haveFile2Result = false;

std::shared_mutex processResultMutex;
ProcessorResult processResult;

void consoleWorker(std::unique_ptr<IOutputGenerator> consoleOutput) {
    while (!stopLogThread) {
        if (haveLogResult) {
            std::shared_lock lock(processResultMutex);
            consoleOutput->generateReport(processResult);
            haveLogResult = false;
        }
    }
}

void fileWorker1(std::unique_ptr<IOutputGenerator> fileOutput) {
    while (!stopFile1Thread) {
        if (haveFile1Result) {
            std::shared_lock lock(processResultMutex);
            fileOutput->generateReport(processResult);
            haveFile1Result = false;
        }
    }
}

void fileWorker2(std::unique_ptr<IOutputGenerator> fileOutput) {
    while (!stopFile2Thread) {
        if (haveFile2Result) {
            std::shared_lock lock(processResultMutex);
            fileOutput->generateReport(processResult);
            haveFile2Result = false;
        }
    }
}

Processor::Processor(size_t n,
                     std::unique_ptr<std::vector<std::unique_ptr<IOutputGenerator>>> output,
                     ITimeProvider* timeProvider)
{
    m_bulkSize = n;
    m_timeProvider = timeProvider;


    for (auto& outputGenerator : *output) {
        switch (outputGenerator->type()) {
            case IOutputGenerator::Type::Unknown: {
                break;
            }

            case IOutputGenerator::Type::Test: {
                m_outputTestGenerators.emplace_back(std::move(outputGenerator));
                break;
            }

            case IOutputGenerator::Type::Console: {
                m_log = std::thread(consoleWorker, std::move(outputGenerator));
                break;
            }

            case IOutputGenerator::Type::File: {
                if (!m_file1.joinable()) {
                    m_file1 = std::thread(fileWorker1, std::move(outputGenerator));
                } else {
                    m_file2 = std::thread(fileWorker2, std::move(outputGenerator));
                }
                break;
            }
        }
    }
}

Processor::~Processor()
{
    stopLogThread = true;
    if (m_log.joinable()) {
        m_log.join();
    }

    stopFile1Thread = true;
    if (m_file1.joinable()) {
        m_file1.join();
    }

    stopFile2Thread = true;
    if (m_file2.joinable()) {
        m_file2.join();
    }
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

    for (const auto& outputGenerator : m_outputTestGenerators) {
        outputGenerator->generateReport(m_processResult);
    }

    {
        std::unique_lock lock(processResultMutex);
        processResult = m_processResult;
    }

    haveLogResult = true;
    if (m_fileSwitcher) {
        haveFile1Result = true;
        m_fileSwitcher = false;
    } else {
        haveFile2Result = true;
        m_fileSwitcher = true;
    }

    clearResults();
}

void Processor::clearResults()
{
    m_processResult.commandsResult.clear();
}
