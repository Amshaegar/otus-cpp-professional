#pragma once

#include "Processor.hpp"

class FileOutputGenerator : public IOutputGenerator {
public:
    void generateReport(const ProcessorResult& result) override;
    IOutputGenerator::Type type() override;
};
