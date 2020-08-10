#include "stringUtils.h"
#include <algorithm>
#include <cctype>
#include <regex>

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

std::vector<std::string> stringUtils::partition(const string& str, const std::string& delim) {
    unsigned long index = str.find(delim);
    if(str.empty() || index == string::npos) {
        return {str, "", ""};
    }
    return {str.substr(0, index), delim, str.substr(index + 1)};
}

string stringUtils::strip(string str) {
    return lstrip(rstrip(std::move(str)));
}

string stringUtils::lstrip(string str) {
    auto end = find_if_not(str.begin(), str.end(), isspace);
    int index = end - str.begin();
    str.erase(0, index);
    return str;
}

string stringUtils::rstrip(string str) {
    auto end = find_if_not(str.rbegin(), str.rend(), isspace);
    unsigned index = str.rend() - end;
    str.erase( index + 1);
    return str;
}

/**
 * Matches all whitespace and groups the whitespace that is between alphanumeric characters
 * @param str The input string
 * @return  A string with the matched, non-grouped whitespace removed
 */
string stringUtils::removeWhitespace(const string& str) {
    std::regex re(R"((\w\s+\w)|\s+)");
    return std::regex_replace(str, re, "$1");
}

bool stringUtils::startsWith(const string& str, const string& prefix) {
    return str.find(prefix) == 0;
}

bool stringUtils::endsWith(const string& str, const string& suffix) {
    return str.rfind(suffix) == str.length() - suffix.length();
}

bool isDigit(const string& str) {
    return std::all_of(str.begin(), str.end(), isdigit);
}

