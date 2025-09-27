#pragma once

#include "Processor.hpp"

#include <ctime>

class TimeProvider : public ITimeProvider {
public:
    time_t getCurrentTimestamp() override;
};
