#define BOOST_TEST_MODULE test_version

#include "lib.h"

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
