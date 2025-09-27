#include "CoutOutputGenerator.hpp"

#include <iostream>

void CoutOutputGenerator::generateReport(const ProcessorResult &result) {
    std::cout << result.toString() << std::endl;
}
