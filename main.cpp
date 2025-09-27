#include "Processor.hpp"
#include "CoutOutputGenerator.hpp"
#include "FileOutputGenerator.hpp"

#include <iostream>
#include <string>

int main(int argc, char *argv[]) {

    if (argc != 2) {
      std::cerr << "Call with static block size: " << argv[0] << " 3\n";
      return 1;
    }

    int N = std::stoi(argv[1]);

    TimeProvider testTimeProvider;
    CoutOutputGenerator* coutOutputGenerator = new CoutOutputGenerator;
    FileOutputGenerator* fileOutputGenerator = new FileOutputGenerator;
    std::vector<IOutputGenerator*> outputGenerators;
    outputGenerators.push_back(coutOutputGenerator);
    outputGenerators.push_back(fileOutputGenerator);

    Processor processor(N, outputGenerators, &testTimeProvider);

    std::string input;
    while(std::getline(std::cin, input)) {
        processor.addCommand(Command(input));
    }

    processor.addCommand(Command("", true));

	return 0;
}
