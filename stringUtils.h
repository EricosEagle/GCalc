#ifndef GCALC_STRINGUTILS_H
#define GCALC_STRINGUTILS_H

#include <string>
#include <vector>

using std::string;

namespace stringUtils {
    std::vector<string> split(string str, const string& delim);
    std::vector<std::string>  partition(const string& str, const std::string& delim);
    string strip(string);
    string lstrip(string);
    string rstrip(string);
    string removeWhitespace(const string&);
    bool isDigit(const string&);
    bool startsWith(const string&, const string&);
    bool endsWith(const string&, const string&);
}

#endif //GCALC_STRINGUTILS_H
