// https://leetcode.com/problems/exclusive-time-of-functions/description/
class Solution {
public:
    vector<int> exclusiveTime(int n, vector<string>& logs) {
        vector<int> res(n, 0);
        int curr;
        int fid;
        char action[6];
        int prev;
        stack<int> st;

        for (auto& log: logs) {
            sscanf(log.c_str(), "%d:%[a-z]:%d", &fid, action, & curr);
            if (action[0] == 's') {
                if (!st.empty()) {
                    res[st.top()] += curr - prev;
                }

                st.push(fid);
                prev = curr;
            } else {
                // Tip: for end, always add 1 to curr, because the end time is inclusive
                int f = st.top();
                st.pop();
                res[f] += curr - prev + 1;
                prev = curr + 1;
            }
        }

        return res;
    }
};