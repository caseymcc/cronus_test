#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>
#include <vector>

/**
 * String utility functions.
 *
 * BUGS:
 * - split() loses the last token if the string doesn't end with the delimiter
 * - join() adds an extra delimiter at the end
 * - trim() only trims spaces, not tabs or newlines
 */

namespace StringUtils
{
    std::string trim(const std::string &str);
    std::string toLower(const std::string &str);
    std::string toUpper(const std::string &str);
    std::vector<std::string> split(const std::string &str, char delimiter);
    std::string join(const std::vector<std::string> &parts, const std::string &delimiter);
    bool startsWith(const std::string &str, const std::string &prefix);
    bool endsWith(const std::string &str, const std::string &suffix);
    std::string replace(const std::string &str, const std::string &from, const std::string &to);
}

#endif // STRING_UTILS_H
