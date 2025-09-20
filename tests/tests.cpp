#define BOOST_TEST_MODULE test_version

#include "lib.h"
#include "ip_filter.h"

#include <iostream>

#include <gtest/gtest.h>

TEST(test_version, test_valid_version) {
    // Arrange
	int actualVersion = 0;
	int expectedThreshold = 0;
    
    // Act
	actualVersion = version();

    // Assert
    EXPECT_TRUE(actualVersion > expectedThreshold);
}

namespace ip_filter
{

TEST(ip_filter, parse) {
    // Arrange
    std::vector<std::string> input = {"1.1.1.1", "1.2.1.1", "1.10.1.1"};
    std::vector<ipv4> expectedOutput = { {1, 1, 1, 1}, {1, 2, 1, 1}, {1, 10, 1, 1} };
    
    // Act
    std::vector<ipv4> actualOutput = parse(input);

    // Assert
    EXPECT_EQ(actualOutput, expectedOutput);
}

TEST(ip_filter, reverse__sort) {
    // Arrange
    std::vector<ipv4> input = { {76, 118, 206, 235}, {171, 4, 234, 224}, {1, 1, 1, 1}, {1, 2, 1, 1}, {1, 10, 1, 1}, {3, 2, 4, 5}, {100, 100, 100, 100} };
    std::vector<ipv4> expectedOutput = { {171, 4, 234, 224}, {100, 100, 100, 100}, {76, 118, 206, 235}, {3, 2, 4, 5}, {1, 10, 1, 1}, {1, 2, 1, 1}, {1, 1, 1, 1} };
    
    // Act
	std::vector<ipv4> actualOutput = reverse_sort(input);

    // Assert
    EXPECT_EQ(actualOutput, expectedOutput);
}

TEST(ip_filter, filter_first_octect_exact_strategy) {
    // Arrange
    std::vector<ipv4> input = { {1, 1, 1, 1}, {2, 2, 1, 1}, {3, 10, 1, 1} };
    std::vector<ipv4> expectedOutput = { {1, 1, 1, 1} };
    
    // Act
	std::vector<ipv4> actualOutput = filter(input, 1);

    // Assert
    EXPECT_EQ(actualOutput, expectedOutput);
}

TEST(ip_filter, filter_first_octect_any_strategy) {
    // Arrange
    std::vector<ipv4> input = { {42, 2, 3, 4}, {2, 42, 1, 1}, {3, 10, 42, 1}, {3, 10, 55, 42}, {42, 42, 42, 42}, {1, 2, 3, 4} };
    std::vector<ipv4> expectedOutput =  { {42, 2, 3, 4}, {2, 42, 1, 1}, {3, 10, 42, 1}, {3, 10, 55, 42}, {42, 42, 42, 42}};
    
    // Act
	std::vector<ipv4> actualOutput = filter(input, 42, Strategy::Any);

    // Assert
    EXPECT_EQ(actualOutput, expectedOutput);
}

TEST(ip_filter, filter_first_two_octects) {
    // Arrange
    std::vector<ipv4> input = { {1, 1, 1, 1}, {2, 2, 1, 1}, {1, 2, 1, 1} };
    std::vector<ipv4> expectedOutput = { {1, 2, 1, 1} };
    
    // Act
	std::vector<ipv4> actualOutput = filter(input, 1, 2);

    // Assert
    EXPECT_EQ(actualOutput, expectedOutput);
}

} // namespace ip_filter