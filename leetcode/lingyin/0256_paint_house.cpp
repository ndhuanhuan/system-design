// https://leetcode.com/problems/paint-house/
class Solution {
public:
    int minCost(vector<vector<int>>& costs) {
        const int n = costs.size();
        if(n == 0 || costs[0].size() == 0) return 0;

        vector<vector<int>> dp = costs;

        for (int i = 1; i < n; ++i) {
            for (int j = 0; j < 3; ++j) {
                dp[i][j] = min(dp[i-1][(j+1)%3], dp[i-1][(j+2)%3]) + dp[i][j];
            }
        }

        return min({dp[n-1][0], dp[n-1][1], dp[n-1][2]});
    }
};