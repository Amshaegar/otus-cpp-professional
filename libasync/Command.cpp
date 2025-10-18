#include "Command.hpp"

Command::Command(const std::string& command, bool isEof)
{
    m_command = command;
    m_isEof = isEof;
}

std::string Command::command() const
{
    return m_command;
}

bool Command::isEof() const
{
    return m_isEof;
}

std::string Command::execute()
{
    return m_command;
}

bool Command::operator==(const Command &other) const
{
    return m_command == other.m_command &&
           m_isEof == other.m_isEof;
}
