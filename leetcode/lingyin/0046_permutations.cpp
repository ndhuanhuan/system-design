// https://leetcode.com/problems/permutations/description/
// Backtracking with swap: fixes elements position by position
// [0..start-1] = fixed, [start..n-1] = remaining choices
class Solution {
public:
    vector<vector<int>> permute(vector<int>& nums) {
        vector<vector<int>> res;
        helper(nums, 0, res);
        return res;
    }

    void helper(vector<int>& nums, int start, vector<vector<int>>& res) {
        if (start >= nums.size()) res.push_back(nums);  // Base case: all positions fixed
        for (int i = start; i < nums.size(); ++i) {
            swap(nums[start], nums[i]);   // Try element i at position start
            helper(nums, start+1, res);   // Fix remaining positions
            swap(nums[start], nums[i]);   // Backtrack: restore original order
        }
    }
};