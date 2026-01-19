// https://leetcode.com/problems/kth-largest-element-in-an-array/description/
class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {
        int n = nums.size();

        int left = 0, right = n-1;
        while(true) {
            int cur = helper(nums, left, right);
            if (cur == k - 1) return nums[cur];
            else if (cur > k-1) right = cur - 1;
            else left = cur + 1;
        }

        return -1;
    }

    int helper(vector<int>& nums, int left, int right) {
        int pivot = nums[left], l = left + 1, r = right;
        while (l <= r) {
            if (nums[l] < pivot && nums[r] > pivot) {
                swap(nums[l++], nums[r--]);
            }

            if (nums[l] >= pivot) l++;
            if (nums[r] <= pivot) r--;
        }

        swap(nums[left], nums[r]); // any index smaller than r guarantee greater or equal to nums[left]
        return r;
    }


};