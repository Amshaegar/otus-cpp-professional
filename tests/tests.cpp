#define BOOST_TEST_MODULE test_processor

#include "Processor.hpp"

#include <gtest/gtest.h>

class TestTimeProvider : public ITimeProvider {
public:
    size_t getCurrentTimestamp() override {
        return 1;
    }
};

class TestOutputGenerator : public IOutputGenerator {
public:
    std::vector<ProcessorResult> processorResults;
    void generateReport(const ProcessorResult& result) override {
        processorResults.push_back(result);
    }
};

TEST(processor_result, equal) {
    // Arrange
    bool isEqualExpected = true;

    ProcessorResult resultsA;
    resultsA.timestamp_first_command = 123;
    std::vector<std::string> commands;
    commands.emplace_back("cmd1");
    commands.emplace_back("cmd2");
    resultsA.commands = commands;

    ProcessorResult resultsB = resultsA;

    // Act
    bool isEqualActual = resultsA == resultsB;

    // Assert
    EXPECT_EQ(isEqualActual, isEqualExpected);
}

TEST(processor_result, not_equal) {
    // Arrange
    bool isEqualExpected = false;

    ProcessorResult resultsA;
    resultsA.timestamp_first_command = 123;
    std::vector<std::string> commands;
    commands.emplace_back("cmd1");
    commands.emplace_back("cmd2");
    resultsA.commands = commands;

    ProcessorResult resultsB;
    resultsB.timestamp_first_command = 456;
    commands.emplace_back("cmd3");
    resultsB.commands = commands;

    // Act
    bool isEqualActual = resultsA == resultsB;

    // Assert
    EXPECT_EQ(isEqualActual, isEqualExpected);
}

TEST(command_processor, static_process_block) {
    // Arrange
    int n = 3;

    TestTimeProvider testTimeProvider;
    TestOutputGenerator* testOutputGenerator = new TestOutputGenerator;
    std::vector<IOutputGenerator*> outputGenerators;
    outputGenerators.push_back(testOutputGenerator);

    Processor processor(n, outputGenerators, &testTimeProvider);

    std::vector<std::string> input;
    input.emplace_back("cmd1");
    input.emplace_back("cmd2");
    input.emplace_back("cmd3");

    ProcessorResult expectedResult;
    expectedResult.timestamp_first_command = testTimeProvider.getCurrentTimestamp();
    expectedResult.commands = input;
    std::vector<ProcessorResult> expectedResults;
    expectedResults.push_back(expectedResult);

    // Act
    for (const std::string& command : input) {
        processor.addCommand(command);
    }

    // Assert
    EXPECT_EQ(testOutputGenerator->processorResults, expectedResults);
}

TEST(command_processor, static_process_block_n_blocks) {
    // Arrange
    int n = 3;

    TestTimeProvider testTimeProvider;
    TestOutputGenerator* testOutputGenerator = new TestOutputGenerator;
    std::vector<IOutputGenerator*> outputGenerators;
    outputGenerators.push_back(testOutputGenerator);

    Processor processor(n, outputGenerators, &testTimeProvider);

    std::vector<std::string> input;
    input.emplace_back("cmd1");
    input.emplace_back("cmd2");
    input.emplace_back("cmd3");
    input.emplace_back("cmd4");
    input.emplace_back("cmd5");
    input.emplace_back("cmd6");

    ProcessorResult expectedResult1;
    expectedResult1.timestamp_first_command = testTimeProvider.getCurrentTimestamp();
    expectedResult1.commands.assign(input.begin(), input.begin() + 3);

    ProcessorResult expectedResult2;
    expectedResult2.timestamp_first_command = testTimeProvider.getCurrentTimestamp();
    expectedResult2.commands.assign(input.begin() + 3, input.end());

    std::vector<ProcessorResult> expectedResults;
    expectedResults.push_back(expectedResult1);
    expectedResults.push_back(expectedResult2);

    // Act
    for (const std::string& command : input) {
        processor.addCommand(command);
    }

    // Assert
    EXPECT_EQ(testOutputGenerator->processorResults, expectedResults);
}

