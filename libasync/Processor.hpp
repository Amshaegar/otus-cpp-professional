#pragma once

#include <memory>
#include <string>
#include <thread>
#include <vector>

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

    enum class Type {
        Unknown = 0,
        Test,
        Console,
        File
    };

    virtual void generateReport(const ProcessorResult& result) = 0;
    virtual Type type() = 0;

    virtual ~IOutputGenerator(){}
};

class Processor {
public:
    explicit Processor(size_t n,
                       std::unique_ptr<std::vector<std::unique_ptr<IOutputGenerator>>> output,
                       ITimeProvider *timeProvider);
    ~Processor();

    Processor(const Processor& other) = delete;
    Processor(Processor&& other) = delete;
    Processor& operator=(const Processor& other) = delete;
    Processor& operator=(Processor&& other) = delete;

    void addCommand(std::unique_ptr<ICommand> command);

private:
    bool isInnerBlock();
    void endBlock();
    void clearResults();

    ProcessorResult m_processResult;

    bool m_isDynamicBlock = false;
    size_t m_innerBlock = 0;
    size_t m_bulkSize = 0;
    std::vector<std::unique_ptr<IOutputGenerator>> m_outputTestGenerators;
    ITimeProvider* m_timeProvider;

    std::thread m_log;
    std::thread m_file1;
    std::thread m_file2;

    bool m_fileSwitcher = false;
};
