// https://leetcode.com/problems/partition-to-k-equal-sum-subsets/
class Solution {
public:
    bool canPartitionKSubsets(vector<int>& nums, int k) {
        const int n = nums.size();
        int sum = accumulate(nums.begin(), nums.end(), 0);
        if (n < k || sum%k) return false;
        vector<bool> visited(n, false);

        int unvisited = n;

        return dfs(nums, visited, sum / k, 0, 0, k, unvisited);

    }

    bool dfs(vector<int>& nums, vector<bool>& visited, int target, int curSum, int i , int k, int& unvisited) {
        if (k == 1) return true;
        if (unvisited == 0 || i >= nums.size()) return false;

        if (curSum == target) {
            return dfs(nums, visited, target, 0, 0, k-1, unvisited);
        }

        for (int j = i; j < nums.size(); j++) {
            if (visited[j] || curSum + nums[j] > target) continue;
            visited[j] = true;
            unvisited--;
            if (dfs(nums, visited, target, curSum + nums[j], j + 1, k, unvisited)) return true;

            unvisited++;
            visited[j] = false;
        }

        return false;
    }
};