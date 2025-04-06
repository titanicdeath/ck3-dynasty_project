#ifndef STRING_UTIL_cPP
#define STRING_UTIL_CPP
#include <windows.h>
#include <psapi.h>
#include <stdexcept>
#include <sstream>   // if needed
#include <vector>
#include <string>
#include <optional>

#include "../library/string_utill.hpp"

using namespace std;

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
    std::size_t start = 0;
    while (start < s.size() && isspace((unsigned char)s[start])) {
        start++;
    }
    // Find last non-whitespace
    std::size_t end = s.size();
    while (end > start && isspace((unsigned char)s[end - 1])) {
        end--;
    }
    return s.substr(start, end - start);
}


bool keyCheck(string_view current, const string check) {
    int total = 0;
    if (current[1] == check[1]){ 
        for (int x = 0; x < check.length(); x++){
            if (current[x] == check[x])total++;
        }
    }
    if (total == check.size()){
        return true;
    }
    else {
        return false;
    }
}

struct Date {
    int year = 0;
    int month = 0;
    int day = 0;
    string toString() const {
         return std::to_string(year) + "." + std::to_string(month) + "." + std::to_string(day);
    }
};

struct Character {
    int id = -1; // The main ID (e.g., 37676)

    // Direct attributes from your list
    optional<string> first_name;
    optional<Date> birth_date; // Store parsed date or keep as string: optional<string> birth_date_str;
    optional<string> ethnicity; // Often a key string
    optional<string> faith;     // Often a key string/ID
    optional<int> dynasty_house_id; // Assuming it's an ID linking elsewhere
    optional<string> dna;

    // Relationships (can occur multiple times)
    vector<int> spouse_ids;
    vector<int> children_ids;

    // Attributes often found within nested blocks (like death)
    optional<Date> death_date;
    optional<string> death_reason;

    // Potential other common fields
    optional<int> dynasty_id;
    optional<string> culture;
    optional<string> religion; // Sometimes separate from faith
    bool is_female = false; // Often represented as "female = yes"
    vector<string> traits; // List of trait keys

    // Domain/Titles - This can be complex. Might be direct holdings or landed_titles reference
    vector<int> title_ids; // IDs of held titles

    // Map to store other key-value pairs we haven't explicitly defined
    // Useful for flexibility or less common attributes
    map<string, string> extra_attributes;

    // Constructor
    Character(int char_id) : id(char_id) {}
    Character() = default; // Default constructor might be needed for map usage
};

#endif  
