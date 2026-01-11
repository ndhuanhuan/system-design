// https://leetcode.com/problems/maximum-product-subarray/description/
class Solution {
public:
    int maxProduct(vector<int>& nums) {
        int res = nums[0];
        int l = res, h = res;
        const int n = nums.size();

        for (int i = 1; i < n; ++i) {
            int last_l = l, last_h = h;
            l = min({nums[i], last_l * nums[i], last_h * nums[i]});
            h = max({nums[i], last_l * nums[i], last_h * nums[i]});
            res = max(res, h);
        }

        return res;
    }
};