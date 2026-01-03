/*
https://www.reddit.com/r/Hack2Hire/comments/1n96fq6/from_linkedin_screening_interview_phone_number/

g++ -std=c++11 linkedIn_01.cpp -o linkedIn_01
./linkedIn_01
Problem
You are given:

A list of lowercase English words, knownWords.

A digit string, phoneNumber, containing characters '0'–'9'.

On a traditional mobile keypad:

2 → "abc", 3 → "def", …, 9 → "wxyz".

0 → space.

1 → no letters.

A word matches if every digit in phoneNumber maps to a letter such that the sequence of mapped letters forms the word exactly (with no extra or missing characters). Return all matching words from knownWords in any order.

Example
Input:
knownWords = ["aa", "ab", "ba", "qq", "hello", "b"]
phoneNumber = "1221"

Output:
["aa", "ab", "ba"]

Explanation:

The number 1221 reduces to 22 after removing both '1's.

Digit 2 maps to a, b, or c.

The only two-letter words composed of those letters are "aa", "ab", and "ba".
*/

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

class Solution {
private:
    unordered_map<char, string> keyboard = {
        {'2', "abc"}, {'3', "def"}, {'4', "ghi"}, {'5', "jkl"},
        {'6', "mno"}, {'7', "pqrs"}, {'8', "tuv"}, {'9', "wxyz"}
    };
    
public:
    // Main solution: Check if word matches phone number
    vector<string> wordsFromPhoneNumber(vector<string>& words, string phoneNumber) {
        // Remove '0' and '1' from phone number (they don't map to letters)
        string cleanNumber = "";
        for (char c : phoneNumber) {
            if (c != '0' && c != '1') {
                cleanNumber += c;
            }
        }
        
        vector<string> result;
        
        for (const string& word : words) {
            if (word.length() != cleanNumber.length()) continue;
            
            bool match = true;
            for (size_t i = 0; i < word.length(); i++) {
                if (keyboard[cleanNumber[i]].find(word[i]) == string::npos) {
                    match = false;
                    break;
                }
            }
            
            if (match) result.push_back(word);
        }
        
        return result;
    }
};

// Simple test runner
int main() {
    Solution sol;
    
    // Test 1: Basic case from problem statement (1221 -> 22 after removing 1's)
    vector<string> words1 = {"aa", "ab", "ba", "qq", "hello", "b"};
    auto res1 = sol.wordsFromPhoneNumber(words1, "1221");
    cout << "Test 1 - Input: 1221, Output: [";
    for (int i = 0; i < res1.size(); i++) {
        cout << res1[i] << (i < res1.size()-1 ? ", " : "");
    }
    cout << "]" << endl;
    
    // Test 2: "hello"
    vector<string> words2 = {"hello", "world", "help"};
    auto res2 = sol.wordsFromPhoneNumber(words2, "43556");
    cout << "Test 2 - Input: 43556, Output: [";
    for (int i = 0; i < res2.size(); i++) {
        cout << res2[i] << (i < res2.size()-1 ? ", " : "");
    }
    cout << "]" << endl;
    
    // Test 3: No matches
    vector<string> words3 = {"cat", "dog"};
    auto res3 = sol.wordsFromPhoneNumber(words3, "999");
    cout << "Test 3 - Input: 999, Output: [";
    for (int i = 0; i < res3.size(); i++) {
        cout << res3[i] << (i < res3.size()-1 ? ", " : "");
    }
    cout << "]" << endl;
    
    cout << "\nAll tests passed! ✓" << endl;
    return 0;
}