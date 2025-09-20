#include "ip_filter.h"

#include <algorithm>
#include <stdexcept>
#include <iostream>

namespace ip_filter
{

bool ipv4::operator==(const ipv4& other) const {
    return first_octet == other.first_octet &&
           second_octet == other.second_octet &&
           third_octet == other.third_octet &&
           forth_octet == other.forth_octet;
}

bool ipv4::operator>(const ipv4& other) const {
    if (first_octet > other.first_octet) {
        return true;
    } else if (first_octet < other.first_octet) {
        return false;
    } else if (first_octet == other.first_octet) {
        if (second_octet > other.second_octet) {
            return true;
        } else if (second_octet < other.second_octet) {
            return false;
        } else if (second_octet == other.second_octet) {
            if (third_octet > other.third_octet) {
                return true;
            } else if (third_octet < other.third_octet) {
                return false;
            } else if (third_octet == other.third_octet) {
                if (forth_octet > other.forth_octet) {
                    return true;
                } else if (forth_octet < other.forth_octet) {
                    return false;
                } else if (forth_octet == other.forth_octet) {
                    return false;
                }
            }
        }
    }
    return false;
/*
    if (first_octet > other.first_octet) {
        return true;
    } else if (second_octet > other.second_octet) {
        return true;
    } else if (third_octet > other.third_octet) {
        return true;
    } else if (forth_octet > other.forth_octet) {
        return true;
    }
    return false;
*/
}

std::ostream& operator<<(std::ostream& os, ipv4 ipv4_address) {
    os << std::to_string(ipv4_address.first_octet) << '.'
       << std::to_string(ipv4_address.second_octet)  << '.'
       << std::to_string(ipv4_address.third_octet) << '.'
       << std::to_string(ipv4_address.forth_octet);
    return os;
}

std::vector<ipv4> parse(const std::vector<std::string>& ipv4_addresses) {

    std::vector<ipv4> addresses;

    for (const std::string& ipv4_address_string : ipv4_addresses) {
        std::vector<std::string> ipv4_address_vector = split(ipv4_address_string, '.');
        if (ipv4_address_vector.size() != 4) {
            throw std::invalid_argument("Invalid IPv4 address. Requires 4 octets. Actual value: " + ipv4_address_string);
        }

        int first_octet = std::stoi(ipv4_address_vector.at(0));
        if (first_octet < 0 || first_octet > 255) {
            throw std::invalid_argument("Invalid IPv4 address. First octet is out of range: " + std::to_string(first_octet));
        }

        int second_octet = std::stoi(ipv4_address_vector.at(1));
        if (second_octet < 0 || second_octet > 255) {
            throw std::invalid_argument("Invalid IPv4 address. Second octet is out of range: " + std::to_string(second_octet));
        }

        int third_octet = std::stoi(ipv4_address_vector.at(2));
        if (third_octet < 0 || third_octet > 255) {
            throw std::invalid_argument("Invalid IPv4 address. Third octet is out of range: " + std::to_string(third_octet));
        }

        int forth_octet = std::stoi(ipv4_address_vector.at(3));
        if (forth_octet < 0 || forth_octet > 255) {
            throw std::invalid_argument("Invalid IPv4 address. Forth octet is out of range: " + std::to_string(forth_octet));
        }

        ipv4 address;
        address.first_octet = first_octet;
        address.second_octet = second_octet;
        address.third_octet = third_octet;
        address.forth_octet = forth_octet;
        addresses.push_back(address);
    }
    return addresses;
}

std::vector<ipv4> reverse_sort(const std::vector<ipv4>& ipv4_addresses) {
    std::vector<ipv4> ipv4_addresses_sorted;
    std::copy(ipv4_addresses.begin(), ipv4_addresses.end(), std::back_inserter(ipv4_addresses_sorted));
    std::stable_sort(ipv4_addresses_sorted.begin(), ipv4_addresses_sorted.end(), std::greater<ipv4>());
    return ipv4_addresses_sorted;
}

std::vector<ipv4> filter(const std::vector<ipv4>& ipv4_addresses, uint8_t first_octet, Strategy strategy) {
    std::vector<ipv4> ipv4_addresses_filtered;
    switch (strategy)
    {
    case Strategy::Exact:
        std::copy_if(ipv4_addresses.cbegin(), ipv4_addresses.cend(),
            std::back_inserter(ipv4_addresses_filtered),
            [first_octet](ipv4 address) {
               return address.first_octet == first_octet;
            });
        break;
    case Strategy::Any:
        std::copy_if(ipv4_addresses.cbegin(), ipv4_addresses.cend(),
            std::back_inserter(ipv4_addresses_filtered),
            [first_octet](ipv4 address) {
               return address.first_octet == first_octet ||
                      address.second_octet == first_octet ||
                      address.third_octet == first_octet ||
                      address.forth_octet == first_octet;
            });
        break;
    default:
        throw std::invalid_argument("Unknown filter strategy!");
    }

    return ipv4_addresses_filtered;
}

std::vector<ipv4> filter(const std::vector<ipv4>& ipv4_addresses, uint8_t first_octet, uint8_t second_octet) {
    std::vector<ipv4> ipv4_addresses_filtered;
     std::copy_if(ipv4_addresses.cbegin(), ipv4_addresses.cend(),
                 std::back_inserter(ipv4_addresses_filtered),
                 [first_octet, second_octet](ipv4 address) {
                    return address.first_octet == first_octet &&
                           address.second_octet == second_octet;
                });
    return ipv4_addresses_filtered;
}

void print(const std::vector<ipv4>& ipv4_addresses) {
    for (const ipv4 address : ipv4_addresses)
    std::cout << address << '\n';
}

} // namespace ip_filter