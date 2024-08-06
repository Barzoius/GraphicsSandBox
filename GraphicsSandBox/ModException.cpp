#include "ModException.h"
#include <sstream>

ModException::ModException(int line, const char* file) noexcept 
    : 
    line(line), 
    file(file) {}

const char* ModException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << GetOriginString();
    
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* ModException::GetType() const noexcept
{
    return "Chili Exception";
}

int ModException::GetLine() const noexcept
{
    return line;
}

const std::string& ModException::GetFile() const noexcept
{
    return file;
}

std::string ModException::GetOriginString() const noexcept
{
    std::ostringstream oss;
    oss << "[File] " << file << std::endl
        << "[Line] " << line;
    return oss.str();
}
