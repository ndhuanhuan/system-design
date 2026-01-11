// https://leetcode.com/problems/maximum-subarray/description/
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        const int n = nums.size();

        int res = INT_MIN, sum = 0;

        for (int i = 0; i < n; ++i) {
            sum = max(sum + nums[i], nums[i]); // didn't wrote correctly at first, should be max of sum + nums[i] and nums[i]
            res = max(res, sum);
        }

        return res;
    }
};