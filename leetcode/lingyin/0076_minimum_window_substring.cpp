// https://leetcode.com/problems/minimum-window-substring/
class Solution {
public:
    string minWindow(string s, string t) {
        const int m = s.size();
        const int n = t.size();

        vector<int> cnt(128, 0);

        for(auto& c: t) {
            cnt[c]++;
        }

        int left = 0, minLeft = -1, minLen = INT_MAX;
        int total = 0;

        for (int right = 0; right < m; ++right) {
            if(--cnt[s[right]] >= 0) total++;
            while (total == n) {
                if (right - left + 1 < minLen) {
                    minLeft = left;
                    minLen = right - left + 1;
                }

                if (++cnt[s[left++]] > 0) total--;
                // left++;
            }
        }

        return minLeft == -1 ? "" : s.substr(minLeft, minLen);
    }
};