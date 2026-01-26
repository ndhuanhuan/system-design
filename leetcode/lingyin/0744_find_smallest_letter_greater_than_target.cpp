// https://leetcode.com/problems/find-smallest-letter-greater-than-target/description/
// You are given an array of characters letters that is sorted in non-decreasing order, and a character target. There are at least two different characters in letters.

// Return the smallest character in letters that is lexicographically greater than target. If such a character does not exist, return the first character in letters.

 

// Example 1:

// Input: letters = ["c","f","j"], target = "a"
// Output: "c"
// Explanation: The smallest character that is lexicographically greater than 'a' in letters is 'c'.

// KEY POINTS:
// 1. Use <= not <: Need STRICTLY GREATER, so skip equal values (letters[mid] <= target moves l forward)
// 2. Use l % size: When no element > target, l = size (out of bounds), wrap to first element
class Solution {
public:
    char nextGreatestLetter(vector<char>& letters, char target) {
        
        
        int l = 0, r = letters.size();
        
        while(l < r) {
            int mid = l + (r - l) / 2;
            if(letters[mid] <= target) {
                l = mid + 1;
            } else {
                r = mid;
            }
        }
        cout << l << endl;
        return letters[l % letters.size()];
        // return letters[l];
    }
};