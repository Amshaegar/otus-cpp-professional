#pragma once

#include <string>
#include <vector>
#include <memory>

class ITimeProvider {
public:
    virtual size_t getCurrentTimestamp() = 0;
};

// TODO Make interface nad implementation move to separate file
class Command {
public:
    explicit Command(const std::string& command, bool isEof = false);

    bool operator==(const Command& other) const;

    std::string execute();
    bool isEof();

private:
    std::string m_command;
    bool m_isEof = false;
};

// TODO Make interface nad implementation move to separate file
struct ProcessorResult {
    size_t timestamp_first_command;
    std::vector<std::string> commandsResult;

    bool operator==(const ProcessorResult& other) const;
};

class IOutputGenerator {
public:
    virtual void generateReport(const ProcessorResult& result) = 0;
};

class Processor {
public:

    // TODO IOutputGenerator* -> unique_ptr
    explicit Processor(size_t n,
                       const std::vector<IOutputGenerator*>& output,
                       ITimeProvider *timeProvider);

    // TODO Use interface
    void addCommand(Command command);

private:
    bool isInnerBlock();
    void endBlock();
    void clearResults();

    ProcessorResult m_processResult;

    bool m_isDynamicBlock = false;
    size_t m_innerBlock = 0;
    size_t m_bulkSize = 0;
    std::vector<IOutputGenerator*> m_outputGenerators;
    ITimeProvider* m_timeProvider;
};
