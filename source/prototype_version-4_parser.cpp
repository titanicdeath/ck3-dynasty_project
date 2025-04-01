#include <windows.h>
#include <psapi.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <string_view>
#include <stdexcept>
#include <algorithm>
#include <list>
#include <cctype>
#include <iomanip>
#include <map>

using namespace std;
size_t getMemoryUsage();
string loadFileIntoString(const string &filename);
vector<string_view> splitLines(const string &buffer);
void pointerPlayOptimized(const string &filename);

vector<string> findBlock(const vector<string_view> &totalLines, const string &charID);

string formatWithCommas(string numStr) {
    string result;
    int count = 0;
    // Iterate backwards over the number string.
    for (int i = numStr.size() - 1; i >= 0; i--) {
        result.insert(result.begin(), numStr[i]);
        count++;
        // Insert a comma every three digits, unless at the beginning.
        if (count % 3 == 0 && i != 0) {
            result.insert(result.begin(), ',');
        }
    }
    return result;
}

// trim leading/trailing whitespace
string trim(const string &s) {
    // Find first non-whitespace
    size_t start = 0;
    while (start < s.size() && isspace((unsigned char)s[start])) {
        start++;
    }
    // Find last non-whitespace
    size_t end = s.size();
    while (end > start && isspace((unsigned char)s[end - 1])) {
        end--;
    }
    return s.substr(start, end - start);
}

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

// Load the entire file into a single string
string loadFileIntoString(const string &filename) {
    ifstream in(filename, ios::in | ios::binary);
    if (!in) {
        throw runtime_error("Could not open file: " + filename);
    }
    ostringstream contents;
    contents << in.rdbuf();
    return contents.str();
}
// vector<string_view> pointerLexicon(string buffer) {
//     static map<char, int> verification = {
//         {' ', 0},    {'0', 1},    {'1', 2},    {'2', 3},
//         {'3', 4},    {'4', 5},    {'5', 6},    {'6', 7},
//         {'7', 8},    {'8', 9},    {'9', 10},   {'+', 11},
//         {'-', 12},   {'=', 13},   {'/', 14},   {'\\', 15},
//         {'.', 16},   {',', 17},   {'[', 18},   {']', 19},
//         {'{', 20},   {'}', 21},   {'A', 22},   {'a', 23},
//         {'B', 24},   {'b', 25},   {'C', 26},   {'c', 27},
//         {'D', 28},   {'d', 29},   {'E', 30},   {'e', 31},
//         {'F', 32},   {'f', 33},   {'G', 34},   {'g', 35},
//         {'H', 36},   {'h', 37},   {'I', 38},   {'i', 39},
//         {'J', 40},   {'j', 41},   {'K', 42},   {'k', 43},
//         {'L', 44},   {'l', 45},   {'M', 46},   {'m', 47},
//         {'N', 48},   {'n', 49},   {'O', 50},   {'o', 51},
//         {'P', 52},   {'p', 53},   {'Q', 54},   {'q', 55},
//         {'R', 56},   {'r', 57},   {'S', 66},   {'s', 67},
//         {'T', 60},   {'t', 61},   {'U', 62},   {'u', 63},
//         {'V', 64},   {'v', 65},   {'W', 68},   {'w', 69},
//         {'X', 70},   {'x', 71},   {'Y', 72},   {'y', 73},
//         {'Z', 74},   {'z', 75},   {'!', 76},   {'?', 77},
//         {':', 78},   {'\"', 79},  {'\x08', 80}, {'\n', 81},
//         {'\r', 82},  {'\t', 83}
//     };

//     static map<int, char> reversedVerification = {
//         {0, ' '},    {1, '0'},    {2, '1'},    {3, '2'},
//         {4, '3'},    {5, '4'},    {6, '5'},    {7, '6'},
//         {8, '7'},    {9, '8'},    {10, '9'},   {11, '+'},
//         {12, '-'},   {13, '='},   {14, '/'},   {15, '\\'},
//         {16, '.'},   {17, ','},   {18, '['},   {19, ']'},
//         {20, '{'},   {21, '}'},   {22, 'A'},   {23, 'a'},
//         {24, 'B'},   {25, 'b'},   {26, 'C'},   {27, 'c'},
//         {28, 'D'},   {29, 'd'},   {30, 'E'},   {31, 'e'},
//         {32, 'F'},   {33, 'f'},   {34, 'G'},   {35, 'g'},
//         {36, 'H'},   {37, 'h'},   {38, 'I'},   {39, 'i'},
//         {40, 'J'},   {41, 'j'},   {42, 'K'},   {43, 'k'},
//         {44, 'L'},   {45, 'l'},   {46, 'M'},   {47, 'm'},
//         {48, 'N'},   {49, 'n'},   {50, 'O'},   {51, 'o'},
//         {52, 'P'},   {53, 'p'},   {54, 'Q'},   {55, 'q'},
//         {56, 'R'},   {57, 'r'},   {66, 'S'},   {67, 's'},
//         {60, 'T'},   {61, 't'},   {62, 'U'},   {63, 'u'},
//         {64, 'V'},   {65, 'v'},   {68, 'W'},   {69, 'w'},
//         {70, 'X'},   {71, 'x'},   {72, 'Y'},   {73, 'y'},
//         {74, 'Z'},   {75, 'z'},   {76, '!'},   {77, '?'},
//         {78, ':'},   {79, '\''},  {80, '\x08'}, {81, '\n'},
//         {82, '\r'},  {83, '\t'}
//     };
//     vector<string_view> result;
//     string_view line;

//     for (size_t i = 0; i < buffer.size(); i++) {
//         int step = verification.at(buffer[i]);
//         // Get the address of the character from the reversedVerification map.
//         char* ch = &reversedVerification.at(step);
//         // Create a string_view of length 1 from that pointer.
//         string_view aussen(ch, 1);
//         // Use the string_view as needed. For example, add it to the result vector.
//         if (*ch == '\n') result.push_back(aussen);
//     }

//     return result;
// }


vector<string_view> pointerLexicon(const string &buffer) {
    // Static maps for conversion – they persist for the program lifetime.
    static map<char, int> verification = {
        {' ', 0},    {'0', 1},    {'1', 2},    {'2', 3},
        {'3', 4},    {'4', 5},    {'5', 6},    {'6', 7},
        {'7', 8},    {'8', 9},    {'9', 10},   {'+', 11},
        {'-', 12},   {'=', 13},   {'/', 14},   {'\\', 15},
        {'.', 16},   {',', 17},   {'[', 18},   {']', 19},
        {'{', 20},   {'}', 21},   {'A', 22},   {'a', 23},
        {'B', 24},   {'b', 25},   {'C', 26},   {'c', 27},
        {'D', 28},   {'d', 29},   {'E', 30},   {'e', 31},
        {'F', 32},   {'f', 33},   {'G', 34},   {'g', 35},
        {'H', 36},   {'h', 37},   {'I', 38},   {'i', 39},
        {'J', 40},   {'j', 41},   {'K', 42},   {'k', 43},
        {'L', 44},   {'l', 45},   {'M', 46},   {'m', 47},
        {'N', 48},   {'n', 49},   {'O', 50},   {'o', 51},
        {'P', 52},   {'p', 53},   {'Q', 54},   {'q', 55},
        {'R', 56},   {'r', 57},   {'S', 66},   {'s', 67},
        {'T', 60},   {'t', 61},   {'U', 62},   {'u', 63},
        {'V', 64},   {'v', 65},   {'W', 68},   {'w', 69},
        {'X', 70},   {'x', 71},   {'Y', 72},   {'y', 73},
        {'Z', 74},   {'z', 75},   {'!', 76},   {'?', 77},
        {':', 78},   {'\"', 79},  {'\x08', 80}, {'\n', 81},
        {'\r', 82},  {'\t', 83}, {'_', 84}
    };

    static map<int, char> reversedVerification = {
        {0, ' '},    {1, '0'},    {2, '1'},    {3, '2'},
        {4, '3'},    {5, '4'},    {6, '5'},    {7, '6'},
        {8, '7'},    {9, '8'},    {10, '9'},   {11, '+'},
        {12, '-'},   {13, '='},   {14, '/'},   {15, '\\'},
        {16, '.'},   {17, ','},   {18, '['},   {19, ']'},
        {20, '{'},   {21, '}'},   {22, 'A'},   {23, 'a'},
        {24, 'B'},   {25, 'b'},   {26, 'C'},   {27, 'c'},
        {28, 'D'},   {29, 'd'},   {30, 'E'},   {31, 'e'},
        {32, 'F'},   {33, 'f'},   {34, 'G'},   {35, 'g'},
        {36, 'H'},   {37, 'h'},   {38, 'I'},   {39, 'i'},
        {40, 'J'},   {41, 'j'},   {42, 'K'},   {43, 'k'},
        {44, 'L'},   {45, 'l'},   {46, 'M'},   {47, 'm'},
        {48, 'N'},   {49, 'n'},   {50, 'O'},   {51, 'o'},
        {52, 'P'},   {53, 'p'},   {54, 'Q'},   {55, 'q'},
        {56, 'R'},   {57, 'r'},   {66, 'S'},   {67, 's'},
        {60, 'T'},   {61, 't'},   {62, 'U'},   {63, 'u'},
        {64, 'V'},   {65, 'v'},   {68, 'W'},   {69, 'w'},
        {70, 'X'},   {71, 'x'},   {72, 'Y'},   {73, 'y'},
        {74, 'Z'},   {75, 'z'},   {76, '!'},   {77, '?'},
        {78, ':'},   {79, '\''},  {80, '\x08'}, {81, '\n'},
        {82, '\r'},  {83, '\t'}, {84, '_'}, {85, ';'}

    };

    vector<string_view> result;
    // A persistent buffer to accumulate the converted characters.
    // Using a static string ensures the storage lives long enough for the string_views.
    static string fullOutput;
    fullOutput.clear();
    fullOutput.reserve(buffer.size());
    cout << "WHY\n";
    size_t lineStart = 0;  // Mark the beginning of the current line

    for (size_t i = 0; i < buffer.size(); i++) {
        auto it = verification.find(buffer[i]);
        if (it == verification.end()) {
            // If the character isn't found, ignore it.
            continue;
        }
        int step = it->second;
        char mappedChar = reversedVerification.at(step);
        fullOutput.push_back(mappedChar);
    
        // If we've hit a newline, create a view for the current line.
        if (mappedChar == '\n') {
            size_t lineLength = fullOutput.size() - lineStart;
            string_view lineView(fullOutput.data() + lineStart, lineLength);
            result.push_back(lineView);
            lineStart = fullOutput.size();
        }
    }

    // If there is any leftover text without a trailing newline, push that as well.
    if (lineStart < fullOutput.size()) {
        string_view lineView(fullOutput.data() + lineStart, fullOutput.size() - lineStart);
        result.push_back(lineView);
    }

    return result;
}


/*
vector<string_view> splitAndConvertLines(const string &buffer, vector<string> &storage) {
    vector<string_view> result;
    size_t start = 0;
    while (start < buffer.size()) {
        size_t end = buffer.find('\n', start);
        if (end == string::npos)
            end = buffer.size();
        // Trim trailing '\r' if present.
        size_t lineLength = (end > start && buffer[end - 1] == '\r') ? end - start - 1 : end - start;
        
        // Build a new string for this line by converting each character.
        string convertedLine;
        convertedLine.reserve(lineLength);
        for (size_t i = start; i < start + lineLength; ++i) {
            const string_view* svPtr = pointerLexicon(buffer[i]);
            if (svPtr)
                convertedLine.append(*svPtr);
            else
                convertedLine.push_back(buffer[i]); // fallback if not found
        }
        // Store the new string in 'storage' so its lifetime extends beyond this function.
        storage.push_back(move(convertedLine));
        // Create a string_view referencing the stored string.
        result.emplace_back(storage.back());
        
        start = end + 1;
    }
    return result;
}
*/
// Split the buffer into lines using string_view to avoid copying data.
vector<string_view> splitLines(const string &buffer) {
    vector<string_view> lines;
    size_t start = 0;
    while (start < buffer.size()) {
        size_t end = buffer.find('\n', start);
        if (end == string::npos) {
            end = buffer.size();
        }
        // Trim a trailing '\r' if present
        size_t lineLength = (end > start && buffer[end - 1] == '\r') ? end - start - 1 : end - start;
        lines.emplace_back(buffer.data() + start, lineLength);
        start = end + 1;
    }
    return lines;
}
/*
vector<string> findBlock(vector<string_view> totalLines, const string &charID){
    cout << "OPENS FUNCTION\n";
    vector<string> block;
    string current;

    int braceDepth = 0, stretch = 0, startLineNumber = 0;
    const string firstPattern = charID + "={", secondPattern = "\tfirst_name=";
    // bool firstCheck = false, secondCheck = false;
    bool patternCheck = false;

    for (size_t i = 0; i < totalLines.size(); i++){
        if (i ==0) cout << "START FOR LOOP" << endl;
        
        current = string(totalLines[i]);
        if (i + 1 < totalLines.size() && (current == firstPattern) && (totalLines[i+1] == secondPattern)) patternCheck = true;
        // if (current == firstPattern) firstCheck = true;
        // if (totalLines[i+1] == secondPattern) secondCheck = true;
        // if ((firstCheck == true) && (secondCheck == true)) {
        if (patternCheck == true) {
            cout << current << endl;
            cout << "Line Number: " << i << endl;
            cout << "Brace Depth: " << braceDepth << endl << endl;

            if (startLineNumber == 0) startLineNumber = i;
            stretch += 1;
            block.push_back(current);

            // Count any '{' or '}' on this line:
            for (char c : current) {
                if (c == '{') braceDepth++;
                if (c == '}') braceDepth--;
            }

            // If braceDepth returns to 0 or below, we've closed the block.
            if (braceDepth <= 0) {
                break;
            }

            // Safety limit
            if (stretch >= 1000){
                cout << "TOO LONG" << endl;
                break;
            }
        }
    }
    cout << "ENDS FOR LOOP" << endl;
    block.push_back(to_string(startLineNumber));
    block.push_back(to_string(stretch));

    return block;
}
*/

bool firstNameCheck(string_view current, const string check) {
    int total = 0;
    if (current[1] == check[1]){ 
        // cout << "|-------------START-------------|" << endl;
        // cout << "|" << current << " | " << endl << check << "| " << endl;
        for (int x = 0; x < 12; x++){
            if (current[x] == check[x])total++;
            // cout << x << " | " << current[x] << " | " << check[x] << " | " << that << " | " << total << " |" << endl;
        }
        // cout << "|--------------END--------------|" << endl;
        // cout << check.size() << endl;
    }

    if (total == check.size()){
        // cout << "============[PASSED]============" << endl << endl;
        return true;
    }
    else {
        // cout << "============[FAILED]============" << endl << endl;
        return false;
    }
}

vector<string_view> filterCharacterBlocks(const vector<string_view> &allLines){
    // cout << "[DEBUG] fCB 1" << endl;
    vector<string_view> filtered;
    bool inBlock = false;
    bool nameInit = false;
    int braceDepth = 0;

    string blockTypeA = "Character blocks Found";
    string blockTypeB = "Other blocks Found";
    string lineBefore = "Lines BEFORE";
    string lineAfter = "Lines AFTER";
    string *blockTypes[4] = {&blockTypeA, &blockTypeB, &lineBefore, &lineAfter};

    int foundBlockTypeA = 0;
    int foundBlockTypeB = 0;
    int lineNumBefore = allLines.size();
    int lineNumAfter;
    int *counters[4] = {&foundBlockTypeA, &foundBlockTypeB, &lineNumBefore, &lineNumAfter};


    for (size_t i = 0; i < allLines.size(); i++) {
        //cout << "[DEBUG] fCB 2." << i << endl;

        // If we are not in a block yet, check for the two-line pattern:
        //   line i ends with "={"
        //   line i+1 starts with "first_name="
        if (!inBlock) {
            //cout << "[DEBUG] fCB 4." << i << endl;
            if (i + 1 < allLines.size()) {
                //cout << "[DEBUG] fCB 6." << i << endl;
                // Trim left for next line (to ignore leading spaces/tabs)
                // We'll do a small inline function:
                auto ltrim = [](string_view sv) {
                    size_t j = 0;
                    while (j < sv.size() && isspace((unsigned char)sv[j])) {
                        j++;
                    }
                    return sv.substr(j);
                };
                
                // Check if line i ends with "={"
                const string_view &lineA = allLines[i];
                if (lineA.size() >= 2 && lineA.substr(lineA.size() - 2) == "={") {
                    //cout << "[DEBUG] fCB 5." << i << endl;
                    // Check next line for first_name=
                    
                    if (inBlock == false) nameInit = firstNameCheck(allLines[i+1], "\tfirst_name=");
                
                    if (nameInit == true) {
                        foundBlockTypeA++;
                        //cout << "[DEBUG] fCB 7." << i << endl;
                        //cout << "[DEBUG] fCB 7." << i << ":\t" << allLines[i+1] << endl;
                        // This is the start of a character block
                        inBlock = true;
                        braceDepth = 0;

                        // Add line i to filtered, then count braces
                        filtered.push_back(lineA);
                        for (char c : lineA) {
                            if (c == '{') braceDepth++;
                            if (c == '}') braceDepth--;
                        }

                        // If the block opened and closed on the same line
                        if (braceDepth <= 0) {
                            inBlock = false;
                            nameInit = false;
                        }
                        // Done checking; move on
                        continue;
                    }
                    else if (nameInit == false) foundBlockTypeB++;
                    
                }
            }
            // If the pattern didn't match, skip this line
        }
        else {
            //cout << "[DEBUG] fCB 3." << i << endl;
            // We are inside a character block

            filtered.push_back(allLines[i]);
            // Count braces
            for (char c : allLines[i]) {
                if (c == '{') braceDepth++;
                if (c == '}') braceDepth--;
            }
            // If the block closes, stop
            if (braceDepth <= 0) {
                inBlock = false;
                nameInit = false;
            }
        }
    }

    lineNumAfter = filtered.size();
    int colWidthLabel = 25;
    int colWidthValue = 12;

    int start = (((4 + colWidthLabel + colWidthValue) - 5) / 2) + 1;
    int end = (((4 + colWidthLabel + colWidthValue) - 3) / 2) + 3;
    cout << "|" << string((start), '-') << "START" << string((start-1), '-') << "|" << endl;
    //printCenteredBanner("START", totalRowWidth);
    for (int i = 0; i < 4; i++) {
        cout << "| " << left << setw(colWidthLabel) << *blockTypes[i] << " | " << right << setw(colWidthValue) << formatWithCommas(to_string(*counters[i])) << " |\n";
    }
    cout << "|" << string((end), '-') << "END" << string((end-5), '-') << "|" << endl;

    return filtered;
}




vector<string> findBlock(const vector<string_view> &totalLines, const string &charID){
    // The patterns we want to match
    // example: "37676={" and (on next line) "first_name="
    string firstPattern  = charID + "={"; 
    string secondPattern = "first_name="; // ignoring leading tabs/spaces
    
    vector<string> block;
    int braceDepth      = 0;
    // how many lines we've added
    int stretch         = 0;         
    // line index where the block started
    int startLineNumber = -1; 
    bool patternCheck   = false;

    for (size_t i = 0; i < totalLines.size(); i++){
        // Convert string_view to string, then trim
        string line = trim(string(totalLines[i]));
        
        // 1) Check if the next line exists before we do i+1
        if (i + 1 < totalLines.size()){
            // We'll also trim the next line for comparison
            string nextLine = trim(string(totalLines[i+1]));

            // If line matches "37676={" (or ######={"  ) exactly 
            // AND the following line starts with or equals "first_name="
            // then set patternCheck = true and initialize braceDepth to 1
            if (!patternCheck && (line == firstPattern) && (nextLine.rfind(secondPattern, 0) == 0)){
                patternCheck = true;
                braceDepth   = 1;  // we already have an opening brace on this line
                startLineNumber = (int) i; 
                
                // We'll also push this line into the block
                block.push_back(string(totalLines[i]));
                stretch++;
                continue; // jump to next iteration
            }
        }

        // 2) If we've already found our pattern, accumulate lines
        if (patternCheck){
            // If we just matched the first line of the block in the step above,
            // we skip pushing the line again. Otherwise, push the new line.
            if (startLineNumber != (int) i){
                block.push_back(string(totalLines[i]));
                stretch++;
            }

            // Count braces in the current line (untrimmed might matter, but typically it's the same)
            // We'll do it on the untrimmed version for safety:
            const string &rawLine = string(totalLines[i]);
            for (char c : rawLine){
                if (c == '{') braceDepth++;
                if (c == '}') braceDepth--;
            }

            // If braceDepth less than or equal to 0, parser reached the closing brace
            if (braceDepth <= 0){
                break;
            }

            // Safety limit
            if (stretch >= 1000){
                cout << "[STRETCH ERROR] Block too large. Or for loop running loose from missreading Blokcs\n";
                cout << "[STRETCH ERROR] For loop running loose from Blokcs read error\n";
                break;
            }
        }
    }

    // If we never set startLineNumber, that means we never found our pattern,
    // so you can handle that if needed (like returning an empty vector).
    if (startLineNumber < 0){
        // For debugging, you might do:
        cout << "No block found for " << charID << "\n";
    }

    // Optionally, record the start line and number of lines processed
    block.push_back(to_string(startLineNumber));
    block.push_back(to_string(stretch));

    return block;
}

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

#define STRING_MEMORY_LOG memorytable.push_back(byteFormat(getMemoryUsage()));
#define T_SIZE_MEMORY_LOG memoryValues.push_back(getMemoryUsage());
#define FINAL_INDEX memoryValues.size() - 1
#define INCREA_MEMORY_LOG memorytable.push_back(byteFormat(memoryValues[FINAL_INDEX] - memoryValues[FINAL_INDEX-1]));
#define DECREA_MEMORY_LOG memorytable.push_back(byteFormat(memoryValues[FINAL_INDEX-1] - memoryValues[FINAL_INDEX]));
#define MEM_CUR return ;

void memoryDefineCall(vector<vector<string>> &memorytable, vector<size_t> &memoryValues, bool spotLog, bool changeLog){
    string flag = "";
    STRING_MEMORY_LOG;
    T_SIZE_MEMORY_LOG;

    //cout << "\n\n[]\n" << memoryValues[FINAL_INDEX] << " - " << memoryValues[FINAL_INDEX-1] << " = " << memoryValues[FINAL_INDEX] - memoryValues[FINAL_INDEX-1] << endl;

    if (spotLog == true){
        vector<string> spotLast = memorytable[memorytable.size()-3];
        cout << "[DEV] Current Program Memory Allocation:   " << spotLast[spotLast.size()-1] << endl;
    }
    if ((memoryValues[FINAL_INDEX] - memoryValues[FINAL_INDEX-1]) > 0) {
        flag = "+";
        INCREA_MEMORY_LOG;
    }
    else{
        flag = "-";
        DECREA_MEMORY_LOG;
    }
    if (changeLog == true){
        size_t change  = memoryValues[memoryValues.size()-1];
        vector<string> format = byteFormat(change);
        cout << "[DEV] Program Memory  Change:    " << flag << format[1] << endl;
    }
    
    

} 


void pointerPlayOptimized(const string &filename) {
    try {
        vector<vector<string>> memorytable;
        vector<size_t> memoryValues;

        // Record memory usage before loading the file.
        // 1
        STRING_MEMORY_LOG;
        T_SIZE_MEMORY_LOG;
        // Load the entire file into one contiguous string.
        string fileBuffer = loadFileIntoString(filename);
        //cout << "Loaded file of size: " << fileBuffer.size() << endl;
        // Record memory usage after loading the file.
        // 2
        STRING_MEMORY_LOG;      
        T_SIZE_MEMORY_LOG;
        // Split into lines using string_view
        vector<string_view> totallyAssimilatedLines = pointerLexicon(fileBuffer);
        // Record memory usage after splitting lines
        //3
        STRING_MEMORY_LOG;
        T_SIZE_MEMORY_LOG;
        vector<string_view> characterOnly = filterCharacterBlocks(totallyAssimilatedLines);
        //cout <<"Before Function Call" << endl;
        //4
        STRING_MEMORY_LOG;
        T_SIZE_MEMORY_LOG;
        //INCREA_MEMORY_LOG;

        // vector<string> ownedCharacterOnly;
        // for (auto sv : characterOnly) {
        //     ownedCharacterOnly.push_back(string(sv));
        // }
        //5
        // STRING_MEMORY_LOG;
        // T_SIZE_MEMORY_LOG;
        // memoryDefineCall(memorytable, memoryValues, true, true);

        //cout << endl << fileBuffer.capacity() << endl;
        // fileBuffer.erase();
        // fileBuffer.clear();
        // fileBuffer.shrink_to_fit();
        //string().swap(fileBuffer);

        //6
        STRING_MEMORY_LOG;
        T_SIZE_MEMORY_LOG;
        totallyAssimilatedLines.clear();
        totallyAssimilatedLines.shrink_to_fit();
        //7
        STRING_MEMORY_LOG;
        T_SIZE_MEMORY_LOG;

        string charID = "37676";
        // vector<string> block = findBlock(totallyAssimilatedLines, charID);
        vector<string> block = findBlock(characterOnly, charID);
        STRING_MEMORY_LOG;
        T_SIZE_MEMORY_LOG;


        // for (int i = 0; i < 10; i++) {
        //     cout << "Line " << i + 1 << ": " << block[i] << "\n";
        // }

        // for (int i = 0; i < block.size(); i++) {
        //     testTF = firstNameCheck(block[i], "\tfirst_name=");
        // }

        // cout << endl << "------------Non-Owned------------" << endl;
        // for (int i = 0; i < 10; i++) {
        //     cout << "Line " << i + 1 << ": " << characterOnly[i] << "\n";
        // }
        // cout << "Total Lines: " << characterOnly.size() << endl;

        // cout << endl << "\n------------Owned------------" << endl;
        // for (int i = 0; i < 10; i++) {
        //     cout << "Line " << i + 1 << ": " << ownedCharacterOnly[i] << "\n";
        // }
        // cout << "Total Lines: " << ownedCharacterOnly.size() << endl;

        // cout << endl << "\n------------Total------------" << endl;
        // for (int i = 0; i < 10; i++) {
        //     cout << "Line " << i + 1 << ": " << totallyAssimilatedLines[i] << "\n";
        // }
        // cout << "Total Lines: " << totallyAssimilatedLines.size() << endl;

        // cout << endl << "\n------------Buffer------------" << endl;
        // for (int i = 0; i < 10; i++) {
        //     cout << "Line " << i + 1 << ": " << fileBuffer[i] << "\n";
        // }
        // cout << "Total Lines: " << fileBuffer.size() << endl;
        
        

        vector<string> row;
        for (int i = 0; i < (memorytable.size()); i++) {
            row = memorytable[i];
            cout << "\n|   [" << i + 1 << "]\t|\t";
            cout << setw(20) << row[2] << "   |   ";
            // for (int x = 0; x < (row.size()); x++) {
            //     cout << setw(20) << row[x] << "   |   ";
            // }
        }
        cout << "\n\n\n\n\n";


    // for (int i = 0; i < (memorytable.size()); i++) {
    //     memoryDefineCall(memorytable, memoryValues, false, false);
    //     row = memorytable[2];
    //     cout << "\n|   [" << i + 1 << "]\t|\t";
    //     for (int x = 0; x < (row.size()); x++) {
    //         cout << setw(20) << row[x] << "   |   ";
    //     }
    // }



    } catch (const exception &ex) {
        cerr << "Error: " << ex.what() << "\n";
    }

    

}

void pause(){
    string fuck;
    cout << "\nEnter anything\n> ";
    cin >> fuck;
}

int main() {



    string inputFile = "../gamestate";
    pointerPlayOptimized(inputFile);


    return 0;
}
