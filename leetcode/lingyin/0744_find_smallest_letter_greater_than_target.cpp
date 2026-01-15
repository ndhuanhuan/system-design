// https://leetcode.com/problems/find-smallest-letter-greater-than-target/description/
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