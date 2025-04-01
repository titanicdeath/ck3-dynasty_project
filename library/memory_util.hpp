#ifndef MEMORY_UTIL_HPP
#define MEMORY_UTIL_HPP

#include <cstddef>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

vector<string> byteFormat(size_t bytes);
size_t getMemoryUsage();



#endif