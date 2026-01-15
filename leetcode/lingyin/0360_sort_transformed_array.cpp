// https://leetcode.com/problems/sort-transformed-array/
class Solution {
public:
    vector<int> sortTransformedArray(vector<int>& nums, int a, int b, int c) {
        const int n = nums.size();
        int l = 0, r = n - 1;
        vector<int> res(n);

        int idx = (a >= 0) ? n-1:0;

        while (l <= r) {

            if (a >= 0) {
                res[idx--] = cal(nums[l], a, b, c) >= cal(nums[r], a, b, c) ? cal(nums[l++], a, b, c) : cal(nums[r--], a, b, c);
            } else {
                res[idx++] = cal(nums[l], a, b, c) >= cal(nums[r], a, b, c) ? cal(nums[r--], a, b, c) : cal(nums[l++], a, b, c);
            }
        }

        return res;
    }

private:
    int cal(int x, int a, int b, int c) {
        return a*x*x + b*x + c;
    }
};