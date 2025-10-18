#include "FileOutputGenerator.hpp"

#include <fstream>
#include <sstream>

void FileOutputGenerator::generateReport(const ProcessorResult &result) {
    std::ostringstream thread_id;
    thread_id << std::this_thread::get_id();

    std::string fileName = "bulk";
    fileName += std::to_string(result.timestamp_first_command);
    fileName += "_" + thread_id.str();
    fileName += ".log";
    std::ofstream bulkFile;
    bulkFile.open (fileName);
    bulkFile << result.toString();
    bulkFile.close();
}

IOutputGenerator::Type FileOutputGenerator::type()
{
    return IOutputGenerator::Type::File;
}
