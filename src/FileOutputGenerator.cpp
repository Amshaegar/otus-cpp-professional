#include "FileOutputGenerator.hpp"

#include <fstream>

void FileOutputGenerator::generateReport(const ProcessorResult &result) {
    std::string fileName = "bulk";
    fileName += std::to_string(result.timestamp_first_command);
    fileName += ".log";
    std::ofstream bulkFile;
    bulkFile.open (fileName);
    bulkFile << result.toString();
    bulkFile.close();
}
