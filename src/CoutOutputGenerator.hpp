#pragma once

#include "Processor.hpp"

class CoutOutputGenerator : public IOutputGenerator {
public:
    void generateReport(const ProcessorResult& result) override;
};
