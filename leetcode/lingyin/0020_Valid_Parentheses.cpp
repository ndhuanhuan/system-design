// https://leetcode.com/problems/valid-parentheses/
class Solution {
public:
    bool isValid(string s) {
        stack<char> st;
        unordered_map<char, char> m{{')', '('}, {']', '['}, {'}', '{'}};
        for (auto& c: s) {
            if (!m.count(c)) {
                st.push(c);
            } else {
                if(st.empty() || st.top() != m[c]) {
                    return false;
                } else {
                    st.pop();
                }
            }
        }

        return st.empty();
    }
};