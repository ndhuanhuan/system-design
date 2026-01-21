// https://leetcode.com/problems/letter-combinations-of-a-phone-number/description/
class Solution {
public:
    vector<string> letterCombinations(string digits) {
        const int n = digits.size();
        if (n == 0) return {};
        vector<string> res;

        vector<vector<char>> d = {
            {' '}, {}, {'a','b','c'}, {'d','e','f'}, {'g','h','i'},
            {'j','k','l'}, {'m','n','o'}, {'p','q','r','s'}, 
            {'t','u','v'}, {'w','x','y','z'}
        };

        string cur;
        helper(digits, 0, d, cur, res);

        return res;
    }

private:
    void helper(string digits, int idx, vector<vector<char>>& d, string& cur, vector<string>& res) {
        if (idx == digits.size()) {
            res.push_back(cur);
            return;
        }

        for (auto& c: d[digits[idx] - '0']) {
            cur.push_back(c);
            helper(digits, idx+1, d, cur, res);
            cur.pop_back();
        }
    }
};