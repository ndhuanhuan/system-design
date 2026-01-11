// https://leetcode.com/problems/shortest-word-distance/description/
class Solution {
public:
    int shortestDistance(vector<string>& wordsDict, string word1, string word2) {
        int res = INT_MAX;
        const int n = wordsDict.size();
        int last1 = -1, last2 = -1;
        for (int i = 0; i < n; ++i) {
            string cur = wordsDict[i];

            if (cur == word1) {
                last1 = i;
            } else if (cur == word2) {
                last2 = i;
            }

            if (last1 != -1 && last2 !=-1) {
                res = min(abs(last1 - last2), res);
            }
        }

        return res;
    }
};