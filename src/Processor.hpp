#pragma once

#include <string>
#include <vector>
#include <memory>

#include <iostream>

class ITimeProvider {
public:
    virtual time_t getCurrentTimestamp() = 0;
};

class ICommand {
public:
    virtual std::string command() const = 0;
    virtual bool isEof() const = 0;

    virtual std::string execute() = 0;

    virtual ~ICommand(){}
};

struct ProcessorResult {
    time_t timestamp_first_command;
    std::vector<std::string> commandsResult;

    bool operator==(const ProcessorResult& other) const;

    std::string toString() const;
};

class IOutputGenerator {
public:
    virtual void generateReport(const ProcessorResult& result) = 0;

    virtual ~IOutputGenerator(){}
};

class Processor {
public:
    explicit Processor(size_t n,
                       std::unique_ptr<std::vector<std::unique_ptr<IOutputGenerator>>> output,
                       ITimeProvider *timeProvider);

    void addCommand(std::unique_ptr<ICommand> command);

private:
    bool isInnerBlock();
    void endBlock();
    void clearResults();

    ProcessorResult m_processResult;

    bool m_isDynamicBlock = false;
    size_t m_innerBlock = 0;
    size_t m_bulkSize = 0;
    std::unique_ptr<std::vector<std::unique_ptr<IOutputGenerator>>> m_outputGenerators;
    ITimeProvider* m_timeProvider;
};
