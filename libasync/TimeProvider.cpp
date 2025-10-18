#include "TimeProvider.hpp"

time_t TimeProvider::getCurrentTimestamp() {
    return std::time(nullptr);
}
