#pragma once

#include <string>
#include <vector>
#include <memory>

class ITimeProvider {
public:
    virtual size_t getCurrentTimestamp() = 0;
};

class ProcessorResult {
public:
    size_t timestamp_first_command;
    std::vector<std::string> commands;

    bool operator==(const ProcessorResult& other) const;
};

class IOutputGenerator {
public:
    virtual void generateReport(const ProcessorResult& result) = 0;
};

class Processor {
public:

    explicit Processor(size_t n,
                       const std::vector<IOutputGenerator*>& output,
                       ITimeProvider *timeProvider);

    void addCommand(const std::string& command);

private:
    void clearResults();

    ProcessorResult m_processResult;

    size_t m_bulkSize = 0;
    std::vector<IOutputGenerator*> m_outputGenerators;
    ITimeProvider* m_timeProvider;
};
