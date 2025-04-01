#include "memory_utill.hpp"
#include "string_utill.hpp"

#include <windows.h>
#include <psapi.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <map>
#include <iostream>

using namespace std;

vector<string> byteFormat(size_t bytes){
    size_t operation = 0;
    size_t place[5] = {bytes, 0, 0, 0, 0};
    string byteType[5] = {"bytes", "kilobytes", "megabytes", "gigabytes", "petabytes"};\
    vector<string> byteValueType;
    for (size_t i = 0; i < 4; i++){
        if (place[i] >= 1024) {
            operation = place[i];
            operation = operation / 1024;
            place[i+1] = operation;
        }
        else {
            break;
        }
    }
    string formattedValues[5];
    string current, running = "";
    for (size_t i = 0; i < 4; i++){
        current = to_string(place[i]);
        current = formatWithCommas(current);
        formattedValues[i] = current;
    }
    string formatted = "";
    //cout << setw(20) << right;
    for (size_t i = 0; i < 4; i++){
        formatted = formattedValues[i] + " " + byteType[i]; 
        byteValueType.push_back(formatted);
    }
    return byteValueType;
}

// Helper function: Get current memory usage (WorkingSetSize) in bytes.
size_t getMemoryUsage() {
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        size_t memory = pmc.WorkingSetSize;
        //cout << format << endl;
        return memory; // in bytes
    }
    throw runtime_error("GetProcessMemoryInfo failed");
}


/*
########################
OTHER MEMORY FUNCTIONS
########################
*/

void memoryLogging(vector<vector<string>> memorytable){
    vector<string> row;
    map<int, string> verification = {
        {1, "|   [1]\t|\tBefore loading file"},
        {2, "|   [2]\t|\tAfter loading file"},
        {3, "|   [3]\t|\tAfter splitting into lines"},
        {4, "|   [4]\t|\tAfter isolating character blocks"},
        {5, "|   [5]\t|\t++Non-owned character blocks weight"},
        {6, "|   [6]\t|\tAfter creating owned character blocks"},
        {7, "|   [7]\t|\t++Owned character block weight"}
    };

    for (int i = 0; i < (memorytable.size()); i++) {
        row = memorytable[i];
        cout << "\n|   [" << i + 1 << "]\t|\t";
        cout << setw(20) << row[2] << "   |   ";
    }
}

// A simple function to record memory usage into a table
void recordMemoryUsage(vector<vector<string>> &memoryTable, vector<size_t> &memoryValues, bool showSpotLog = false, bool showChangeLog = false) {
    // 1) Get current usage
    size_t current = getMemoryUsage();
    memoryValues.push_back(current);

    // 2) Push it into the table as well
    // For example, the "byteFormat" function returns a vector<string>.
    auto usageStrings = byteFormat(current);
    memoryTable.push_back(usageStrings);

    // Optionally show the user the spot usage
    if (showSpotLog) {
    // usageStrings[1] might be the "kilobytes" string if that’s how you set up byteFormat
    cout << "[DEV] Current Program Memory: " << usageStrings[1] << "\n";
    }

    // If there's a previous measurement, show the difference
    if (showChangeLog && memoryValues.size() > 1) {
    size_t prev = memoryValues[memoryValues.size() - 2];
    size_t delta = current > prev ? current - prev : prev - current;
    auto deltaStrings = byteFormat(delta);
    char sign = (current >= prev) ? '+' : '-';
    cout << "[DEV] Program Memory Change: " << sign << deltaStrings[1] << "\n";
    }
}



// #define STRING_MEMORY_LOG memorytable.push_back(byteFormat(getMemoryUsage()));
// #define T_SIZE_MEMORY_LOG memoryValues.push_back(getMemoryUsage());
// #define FINAL_INDEX memoryValues.size() - 1
// #define INCREA_MEMORY_LOG memorytable.push_back(byteFormat(memoryValues[FINAL_INDEX] - memoryValues[FINAL_INDEX-1]));
// #define DECREA_MEMORY_LOG memorytable.push_back(byteFormat(memoryValues[FINAL_INDEX-1] - memoryValues[FINAL_INDEX]));
// #define MEM_CUR return ;

// void memoryDefineCall(vector<vector<string>> &memorytable, vector<size_t> &memoryValues, bool spotLog, bool changeLog){
//     string flag = "";
//     STRING_MEMORY_LOG;
//     T_SIZE_MEMORY_LOG;

//     //cout << "\n\n[]\n" << memoryValues[FINAL_INDEX] << " - " << memoryValues[FINAL_INDEX-1] << " = " << memoryValues[FINAL_INDEX] - memoryValues[FINAL_INDEX-1] << endl;

//     if (spotLog == true){
//         vector<string> spotLast = memorytable[memorytable.size()-3];
//         cout << "[DEV] Current Program Memory Allocation:   " << spotLast[spotLast.size()-1] << endl;
//     }
//     if ((memoryValues[FINAL_INDEX] - memoryValues[FINAL_INDEX-1]) > 0) {
//         flag = "+";
//         INCREA_MEMORY_LOG;
//     }
//     else{
//         flag = "-";
//         DECREA_MEMORY_LOG;
//     }
//     if (changeLog == true){
//         size_t change  = memoryValues[memoryValues.size()-1];
//         vector<string> format = byteFormat(change);
//         cout << "[DEV] Program Memory  Change:    " << flag << format[1] << endl;
//     }
    
    

// } 