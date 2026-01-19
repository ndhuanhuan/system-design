// https://leetcode.com/problems/number-of-perfect-pairs/description/
// https://leetcode.cn/problems/number-of-perfect-pairs/solutions/3755121/yong-jue-dui-zhi-de-ji-he-yi-yi-hua-jian-xois/
class Solution {
public:
    long long perfectPairs(vector<int>& nums) {
        for (int& x : nums) {
            x = abs(x);
        }

        ranges::sort(nums);
        long long ans = 0;
        int left = 0;
        for (int j = 0; j < nums.size(); j++) {
            int b = nums[j];
            while (nums[left] * 2 < b) {
                left++;
            }
            // a=nums[i]，其中 i 最小是 left，最大是 j-1，一共有 j-left 个
            ans += j - left;
        }
        return ans;
    }
};