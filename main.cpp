#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "ip_filter.h"

int main(int argc, char const *argv[])
{
	(void)argc;
	(void)argv;
    try
    {
        std::vector<std::string> addresses;
        for(std::string line; std::getline(std::cin, line);)
        {
            std::vector<std::string> v = split(line, '\t');
            addresses.push_back(v.at(0));
        }
        std::vector<ip_filter::ipv4> ip_pool;
        ip_pool = ip_filter::parse(addresses);
        std::vector<ip_filter::ipv4> ip_pool_reverse_sorted = ip_filter::reverse_sort(ip_pool);
        ip_filter::print(ip_pool_reverse_sorted);
        ip_filter::print(ip_filter::filter(ip_pool_reverse_sorted, 1, ip_filter::Strategy::Exact));
        ip_filter::print(ip_filter::filter(ip_pool_reverse_sorted, 46, 70));
        ip_filter::print(ip_filter::filter(ip_pool_reverse_sorted, 46, ip_filter::Strategy::Any));
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
