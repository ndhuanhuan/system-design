// https://leetcode.com/problems/find-first-and-last-position-of-element-in-sorted-array/description/
class Solution {
public:
    vector<int> searchRange(vector<int>& nums, int target) {
        int start = helper(nums, target);
        if (start == nums.size() || nums[start] != target) return {-1, -1};
        return {start, helper(nums, target+1) -1};
    }

    int helper(vector<int>& nums, int target) {
        int l = 0, r = nums.size();

        while (l < r) {
            int mid = l + (r - l) / 2;
            if (nums[mid] < target) l = mid + 1;
            else r = mid;
        }

        return l;
    }
};