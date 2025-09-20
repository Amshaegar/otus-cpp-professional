#pragma once

#include "util.h"

#include <cstdint>
#include <vector>
#include <string>
#include <ostream>

namespace ip_filter
{

class ipv4 {
public:
    uint8_t first_octet;
    uint8_t second_octet;
    uint8_t third_octet;
    uint8_t forth_octet;

    bool operator==(const ipv4& other) const;
    bool operator>(const ipv4& other) const;
};

std::ostream& operator<<(std::ostream& os, ipv4 ipv4_address);

enum class Strategy {
    Exact = 0,
    Any
};

std::vector<ipv4> parse(const std::vector<std::string>& ipv4_addresses);

std::vector<ipv4> reverse_sort(const std::vector<ipv4>& ipv4_addresses);

std::vector<ipv4> filter(const std::vector<ipv4>& ipv4_addresses, uint8_t first_octet, Strategy strategy = Strategy::Exact);

std::vector<ipv4> filter(const std::vector<ipv4>& ipv4_addresses, uint8_t first_octet, uint8_t second_octet);

void print(const std::vector<ipv4>& ipv4_addresses);

} // namespace ip_filter