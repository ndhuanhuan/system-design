// https://leetcode.com/problems/max-consecutive-ones-iii/
class Solution {
public:
    int longestOnes(vector<int>& nums, int k) {
        int start = 0,  zeroCnt = 0, res = INT_MIN;
        const int n = nums.size();
        for (int end = 0; end < n; ++end) {
            if(nums[end] == 0) {
                zeroCnt++;
            }

            while(zeroCnt > k) {
                if(nums[start] == 0) {
                    zeroCnt--; 
                }
                start++;
            }

            res = max(res, end - start + 1);
        }

        return res;
    }
};