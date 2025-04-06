#ifndef STRING_UTIL_HPP
#define STRING_UTIL_HPP

#include <iomanip>
#include <iostream>
#include <string>


using namespace std;

string formatWithCommas(string numStr);
string trim(const string &s);
bool keyCheck(string_view current, const string check);
#endif  