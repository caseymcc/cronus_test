#include "string_utils.h"
#include <algorithm>
#include <sstream>

namespace StringUtils
{

std::string trim(const std::string &str)
{
    // BUG: Only trims spaces, not tabs or newlines
    size_t start = str.find_first_not_of(' ');
    size_t end = str.find_last_not_of(' ');

    if(start == std::string::npos)
    {
        return "";
    }

    return str.substr(start, end - start + 1);
}

std::string toLower(const std::string &str)
{
    std::string result = str;

    std::transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c) { return std::tolower(c); });

    return result;
}

std::string toUpper(const std::string &str)
{
    std::string result = str;

    std::transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c) { return std::toupper(c); });

    return result;
}

std::vector<std::string> split(const std::string &str, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream stream(str);

    while(std::getline(stream, token, delimiter))
    {
        tokens.push_back(token);
    }

    // BUG: getline actually handles this correctly for most cases,
    // but this implementation drops empty trailing tokens.
    // The real bug: if str is empty, returns empty vector (should return [""])
    // Actually, let's make the bug more obvious:
    // We incorrectly remove the last element if it's empty
    if(!tokens.empty() && tokens.back().empty())
    {
        tokens.pop_back();
    }

    return tokens;
}

std::string join(const std::vector<std::string> &parts, const std::string &delimiter)
{
    std::string result;

    // BUG: Always adds delimiter after each part, including the last
    for(const auto &part : parts)
    {
        result += part + delimiter;
    }

    return result;
}

bool startsWith(const std::string &str, const std::string &prefix)
{
    if(prefix.size() > str.size())
    {
        return false;
    }

    return str.compare(0, prefix.size(), prefix) == 0;
}

bool endsWith(const std::string &str, const std::string &suffix)
{
    if(suffix.size() > str.size())
    {
        return false;
    }

    return str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

std::string replace(const std::string &str, const std::string &from, const std::string &to)
{
    std::string result = str;
    size_t pos = 0;

    while((pos = result.find(from, pos)) != std::string::npos)
    {
        result.replace(pos, from.length(), to);
        pos += to.length();
    }

    return result;
}

} // namespace StringUtils
