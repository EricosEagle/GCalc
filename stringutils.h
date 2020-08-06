#ifndef GCALC_STRINGUTILS_H
#define GCALC_STRINGUTILS_H

#include <string>
#include <vector>

using std::string;

namespace stringUtils {
    std::vector<string> split(string str, const string& delim);
    std::pair<string, string> seperate(const string& str, char delim);
    string strip(string);
    string lstrip(string);
    string rstrip(string);
    string removeWhitespace(const string&);
    string toLower(const string&);
    string toUpper(const string&);
    bool startsWith(const string&, const string&);
    bool endsWith(const string&, const string&);
}

#endif //GCALC_STRINGUTILS_H
