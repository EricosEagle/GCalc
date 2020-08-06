#include "stringutils.h"
#include <algorithm>
#include <cctype>
#include <regex>
#include <utility>

std::vector<string> stringUtils::split(string str, const string& delim) {
    std::vector<string> out;
    size_t pos;
    do {
        pos = str.find(delim);
        out.push_back(str.substr(0, pos));
        str.erase(0, pos + delim.length());
    } while (pos != string::npos);
    return out;
}

std::pair<string, string> stringUtils::seperate(const string& str, char delim) {
    unsigned long index = str.find(delim);
    if(index == string::npos) {
        throw std::invalid_argument("The delimiter was not found in the string.");
    } else if(index != str.rfind(delim)) {
        throw std::invalid_argument("The delimiter is in the string more than one time.");
    }
    return std::pair<string, string>(str.substr(0, index), str.substr(index + 1));
}

string stringUtils::strip(string str) {
    return lstrip(rstrip(std::move(str)));
}

string stringUtils::lstrip(string str) {
    string::const_iterator end = find_if_not(str.begin(), str.end(), isspace);
    int index = end - str.begin();
    str.erase(0, index);
    return str;
}

string stringUtils::rstrip(string str) {
    string::const_reverse_iterator end = find_if_not(str.rbegin(), str.rend(), isspace);
    int index = str.rend() - end;
    if(index < (int) str.length() - 1) {
        str.erase( index + 1, str.length());
    }
    return str;
}

/**
 * Matches all whitespace and groups the whitespace that is between alphanumeric characters
 * @param str The input string
 * @return  A string with the matched, non-grouped whitespace removed
 */
string stringUtils::removeWhitespace(const string& str) {
    std::regex re("(\\w\\s+\\w)|\\s+");
    return std::regex_replace(str, re, "$1");
}

string stringUtils::toUpper(const string& str) {
    string out;
    std::transform(str.begin(), str.end(), out.begin(), toupper);
    return out;
}

string stringUtils::toLower(const string& str) {
    string out;
    std::transform(str.begin(), str.end(), out.begin(), tolower);
    return out;
}

bool stringUtils::startsWith(const string& str, const string& prefix) {
    return str.find(prefix) == 0;
}

bool stringUtils::endsWith(const string& str, const string& suffix) {
    return str.rfind(suffix) == str.length() - suffix.length();
}

