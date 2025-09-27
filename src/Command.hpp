#pragma once

#include "Processor.hpp"

class Command : public ICommand {
public:
    explicit Command(const std::string& command, bool isEof = false);

    std::string command() const override;
    bool isEof() const override;

    std::string execute() override;

    bool operator==(const Command& other) const;

private:
    std::string m_command;
    bool m_isEof = false;
};
