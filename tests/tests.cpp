#define BOOST_TEST_MODULE test_processor

#include "Processor.hpp"
#include "Command.hpp"

#include <gtest/gtest.h>

class TestTimeProvider : public ITimeProvider {
public:
    time_t getCurrentTimestamp() override {
        return currentTimeStamp;
    }

    static time_t currentTimeStamp;
};

time_t TestTimeProvider::currentTimeStamp = 1;

class TestOutputGenerator : public IOutputGenerator {
public:
    static std::vector<ProcessorResult> processorResults;
    void generateReport(const ProcessorResult& result) override {
        processorResults.push_back(result);
    }

    IOutputGenerator::Type type() override {
        return IOutputGenerator::Type::Test;
    }
};

std::vector<ProcessorResult> TestOutputGenerator::processorResults;

class ProcessorResultEqual : public ::testing::TestWithParam<std::tuple<ProcessorResult, ProcessorResult, bool>> {};
INSTANTIATE_TEST_SUITE_P(ProcessorResultEqualValues,
                         ProcessorResultEqual,
                         testing::Values(
                             std::make_tuple(ProcessorResult{123, {"cmd1", "cmd2"}},
                                             ProcessorResult{123, {"cmd1", "cmd2"}},
                                             true),
                             std::make_tuple(ProcessorResult{123, {"cmd1", "cmd2"}},
                                             ProcessorResult{456, {"cmd3"}},
                                             false)
                         ));

TEST_P(ProcessorResultEqual, ProcessorResultEqualCheck) {
    // Arrange
    auto [resultsA, resultsB, isEqualExpected] = GetParam();

    // Act
    bool isEqualActual = resultsA == resultsB;

    // Assert
    EXPECT_EQ(isEqualActual, isEqualExpected);
}

class ProcessBlocks : public ::testing::TestWithParam<std::tuple<int, std::vector<Command>, std::vector<ProcessorResult>>> {};
INSTANTIATE_TEST_SUITE_P(ProcessBlocksValues,
                         ProcessBlocks,
                         testing::Values(
                             std::make_tuple(3,
                                             std::vector<Command> {Command("cmd1"), Command("cmd2"), Command("cmd3")},
                                             std::vector<ProcessorResult>{ {TestTimeProvider::currentTimeStamp,
                                                                           {{"cmd1"}, {"cmd2"}, {"cmd3"}}}
                                                                         }),
                             std::make_tuple(3,
                                             std::vector<Command> {Command("cmd1"), Command("cmd2"), Command("", true)},
                                             std::vector<ProcessorResult>{ {TestTimeProvider::currentTimeStamp,
                                                                           {{"cmd1"}, {"cmd2"}}}
                                                                         }),
                             std::make_tuple(3,
                                             std::vector<Command> {Command("cmd1"), Command("cmd2"), Command("cmd3"),
                                                                   Command("cmd4"), Command("cmd5"), Command("cmd6")},
                                             std::vector<ProcessorResult>{ {TestTimeProvider::currentTimeStamp,
                                                                            {{"cmd1"}, {"cmd2"}, {"cmd3"}}},
                                                                           {TestTimeProvider::currentTimeStamp,
                                                                             {{"cmd4"}, {"cmd5"}, {"cmd6"}}}
                                                                         }),
                             std::make_tuple(3,
                                             std::vector<Command> {Command("cmd1"), Command("cmd2"), Command("{")},
                                             std::vector<ProcessorResult>{ {TestTimeProvider::currentTimeStamp,
                                                                           {{"cmd1"}, {"cmd2"} }}
                                                                         }),
                             std::make_tuple(3,
                                             std::vector<Command> {Command("{"), Command("cmd1"), Command("cmd2"), Command("", true)},
                                             std::vector<ProcessorResult>{}),
                             std::make_tuple(3,
                                             std::vector<Command> {Command("cmd1"), Command("cmd2"),
                                                                   Command("{"), Command("cmd3"), Command("cmd4"), Command("}")},
                                             std::vector<ProcessorResult>{ {TestTimeProvider::currentTimeStamp,
                                                                            {{"cmd1"}, {"cmd2"}}},
                                                                           {TestTimeProvider::currentTimeStamp,
                                                                            {{"cmd3"}, {"cmd4"}}}
                                                                         }),
                             std::make_tuple(3,
                                             std::vector<Command> {Command("cmd1"), Command("cmd2"),
                                                                   Command("{"),
                                                                   Command("cmd3"),
                                                                   Command("cmd4"),
                                                                   Command("{"), Command("cmd5"), Command("cmd6"), Command("}"),
                                                                   Command("cmd7"),
                                                                   Command("}")},
                                             std::vector<ProcessorResult>{ {TestTimeProvider::currentTimeStamp,
                                                                            {{"cmd1"}, {"cmd2"}}},
                                                                           {TestTimeProvider::currentTimeStamp,
                                                                            {{"cmd3"}, {"cmd4"}, {"cmd5"}, {"cmd6"}, {"cmd7"}}}
                                                                         })
                         ));

TEST_P(ProcessBlocks, ProcessBlocksCheck) {
    // Arrange
    TestOutputGenerator::processorResults.clear();
    auto [n, input, expectedResults] = GetParam();

    TestTimeProvider testTimeProvider;
    auto outputGenerators = std::make_unique<std::vector<std::unique_ptr<IOutputGenerator>>>();
    outputGenerators->emplace_back(std::make_unique<TestOutputGenerator>());

    Processor processor(n, std::move(outputGenerators), &testTimeProvider);

    // Act
    for (Command& command : input) {
        processor.addCommand(std::make_unique<Command>(command.command(), command.isEof()));
    }

    // Assert
    EXPECT_EQ(TestOutputGenerator::processorResults, expectedResults);
}
