#include "CoutOutputGenerator.hpp"

#include <iostream>

void CoutOutputGenerator::generateReport(const ProcessorResult &result) {
    std::cout << result.toString() << std::endl;
}

IOutputGenerator::Type CoutOutputGenerator::type()
{
    return IOutputGenerator::Type::Console;
}
