// https://leetcode.com/problems/smallest-range-covering-elements-from-k-lists/description/
class Solution {
public:
    vector<int> smallestRange(vector<vector<int>>& nums) {
        int curMax = INT_MIN, n = nums.size();

        vector<int> idx(n, 0);
        auto cmp = [](pair<int, int>& a, pair<int, int>& b) { return a.first > b.first; };

        priority_queue<pair<int,int>, vector<pair<int, int>>, decltype(cmp)> q(cmp);

        for (int i = 0; i < n; ++i) {
            q.push({nums[i][0], i});
            idx[i] = 1;
            curMax = max(curMax, nums[i][0]);
        }

        vector<int> res{q.top().first, curMax};

        while(idx[q.top().second] < nums[q.top().second].size()) {
            auto t = q.top().second; q.pop();
            int newNum = nums[t][idx[t]];
            q.push({newNum, t});
            curMax = max(curMax, newNum);
            idx[t]++;

            if (res[1] - res[0] > curMax - q.top().first) {
                res = {q.top().first, curMax};
            }
        }

        return res;
    }
};