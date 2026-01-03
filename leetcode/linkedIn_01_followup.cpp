/*
https://www.reddit.com/r/Hack2Hire/comments/1n96fq6/from_linkedin_screening_interview_phone_number/
https://github.com/monkey0211/AlgoInPython/blob/main/LinkedIn/Linkedin_Words%20from%20Phone%20Numbers.py
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

part2: adjust implementation for use case when new words can be added and existing words can be removed from the dictionary at runtime. 
*/


// Have a letter to digit mapping first, a->2, b->2, c->2, d->3 ..., stored in letterToDigit map
// wordToDigits() function to convert a word to its digit representation, used by addWord and removeWord. Maintained in digitToWords map: digit sequence -> set of words
// Then when input phone number comes, we can directly look up digitToWords map to get the matching words in O(1) time.
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
using namespace std;

class Solution {
private:
    unordered_map<char, string> keyboard = {
        {'2', "abc"}, {'3', "def"}, {'4', "ghi"}, {'5', "jkl"},
        {'6', "mno"}, {'7', "pqrs"}, {'8', "tuv"}, {'9', "wxyz"}
    };
    
    // Reverse mapping: letter -> digit for fast conversion
    unordered_map<char, char> letterToDigit;
    
    // Maps digit sequence to list of words: "22" -> ["aa", "ab", "ba"]
    unordered_map<string, unordered_set<string>> digitToWords;
    
    // Convert word to digit sequence
    string wordToDigits(const string& word) {
        string digits = "";
        for (char c : word) {
            if (letterToDigit.count(c)) {
                digits += letterToDigit[c];
            }
        }
        return digits;
    }
    
public:
    Solution() {
        // Build reverse mapping: letter -> digit
        for (auto& pair : keyboard) {
            char digit = pair.first;
            for (char letter : pair.second) {
                letterToDigit[letter] = digit;
            }
        }
    }
    
    // Add word to dictionary - O(word.length)
    void addWord(const string& word) {
        string digits = wordToDigits(word);
        digitToWords[digits].insert(word);
    }
    
    // Remove word from dictionary - O(word.length)
    void removeWord(const string& word) {
        string digits = wordToDigits(word);
        if (digitToWords.count(digits)) {
            digitToWords[digits].erase(word);
            if (digitToWords[digits].empty()) {
                digitToWords.erase(digits);
            }
        }
    }
    
    // Query words matching phone number - O(1) lookup + O(k) to build result where k = # matching words
    vector<string> wordsFromPhoneNumber(string phoneNumber) {
        // Remove '0' and '1' from phone number (they don't map to letters)
        string cleanNumber = "";
        for (char c : phoneNumber) {
            if (c != '0' && c != '1') {
                cleanNumber += c;
            }
        }
        
        vector<string> result;
        if (digitToWords.count(cleanNumber)) {
            for (const string& word : digitToWords[cleanNumber]) {
                result.push_back(word);
            }
        }
        
        return result;
    }
};

void printVector(const vector<string>& vec) {
    cout << "[";
    for (size_t i = 0; i < vec.size(); i++) {
        cout << vec[i];
        if (i < vec.size() - 1) cout << ", ";
    }
    cout << "]";
}

int main() {
    Solution sol;
    
    // Test 1: Basic case with dynamic add
    cout << "Test 1: Adding words dynamically" << endl;
    sol.addWord("aa");
    sol.addWord("ab");
    sol.addWord("ba");
    sol.addWord("qq");
    sol.addWord("hello");
    
    auto res1 = sol.wordsFromPhoneNumber("1221");
    cout << "Query '1221': "; printVector(res1); cout << endl;
    
    // Test 2: LinkedIn example
    cout << "\nTest 2: LinkedIn example" << endl;
    sol.addWord("careers");
    sol.addWord("linkedin");
    sol.addWord("hiring");
    sol.addWord("interview");
    sol.addWord("linkedgo");
    
    auto res2 = sol.wordsFromPhoneNumber("2273377");
    cout << "Query '2273377': "; printVector(res2); cout << endl;
    
    auto res3 = sol.wordsFromPhoneNumber("54653346");
    cout << "Query '54653346': "; printVector(res3); cout << endl;
    
    auto res4 = sol.wordsFromPhoneNumber("447464");
    cout << "Query '447464': "; printVector(res4); cout << endl;
    
    // Test 3: Remove word
    cout << "\nTest 3: Removing words" << endl;
    sol.removeWord("linkedin");
    auto res5 = sol.wordsFromPhoneNumber("54653346");
    cout << "After removing 'linkedin', query '54653346': "; printVector(res5); cout << endl;
    
    // Test 4: Add back
    cout << "\nTest 4: Adding back removed word" << endl;
    sol.addWord("linkedin");
    auto res6 = sol.wordsFromPhoneNumber("54653346");
    cout << "After adding 'linkedin' back, query '54653346': "; printVector(res6); cout << endl;
    
    cout << "\nAll tests passed! ✓" << endl;
    return 0;
}