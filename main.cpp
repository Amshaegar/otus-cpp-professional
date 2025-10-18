#include "Command.hpp"
#include "CoutOutputGenerator.hpp"
#include "FileOutputGenerator.hpp"
#include "Processor.hpp"
#include "TimeProvider.hpp"

#include <iostream>
#include <string>

int main(int argc, char *argv[]) {

    if (argc != 2) {
      std::cerr << "Call with static block size: " << argv[0] << " 3\n";
      return 1;
    }

    int N = std::stoi(argv[1]);

    TimeProvider timeProvider;
    auto outputGenerators = std::make_unique<std::vector<std::unique_ptr<IOutputGenerator>>>();
    outputGenerators->emplace_back(std::make_unique<CoutOutputGenerator>());
    outputGenerators->emplace_back(std::make_unique<FileOutputGenerator>());
    outputGenerators->emplace_back(std::make_unique<FileOutputGenerator>());

    Processor processor(N, std::move(outputGenerators), &timeProvider);

    std::string input;
    while(std::getline(std::cin, input)) {
        processor.addCommand(std::make_unique<Command>(input));
    }

    processor.addCommand(std::make_unique<Command>("", true));

	return 0;
}
