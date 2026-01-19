// https://leetcode.com/problems/permutations-ii/description/
// Given a collection of numbers, nums, that might contain duplicates, return all possible unique permutations in any order.

 

// Example 1:

// Input: nums = [1,1,2]
// Output:
// [[1,1,2],
//  [1,2,1],
//  [2,1,1]]
// Example 2:

// Input: nums = [1,2,3]
// Output: [[1,2,3],[1,3,2],[2,1,3],[2,3,1],[3,1,2],[3,2,1]]
 


class Solution {
public:
	vector<vector<int>> permuteUnique(vector<int>& nums) {
		int n = nums.size();
		vector<vector<int>> result;
		vector<int> path;
		// Sort array to group duplicates together
		sort(nums.begin(),nums.end());
		vector<bool> used(nums.size(), false);
		dfs(nums,used, path, result);
		return result;
	}

	void dfs(const vector<int>& nums, vector<bool> &used, vector<int> &path, vector<vector<int> > &result)
	{
		if (path.size() == nums.size()) 
		{
			result.push_back(path);
			return;
		}
		for (int i = 0; i<nums.size(); i++) {
			// Skip if current element is already used in this path
			if (used[i]) continue;
			
			/* DUPLICATE PREVENTION LOGIC:
			 * This line prevents duplicate permutations when array has duplicate elements.
			 * 
			 * Condition breakdown:
			 * - i > 0: Not the first element
			 * - nums[i] == nums[i-1]: Current element equals previous element (duplicates)
			 * - !used[i-1]: Previous duplicate hasn't been used in current path
			 * 
			 * Key insight: For duplicate elements, we ONLY use nums[i] if nums[i-1] 
			 * has already been used. This ensures duplicates are picked in order (left to right)
			 * and prevents generating the same permutation multiple times.
			 * 
			 * Example: nums = [1, 1', 2] (1' marks second 1 for clarity)
			 * 
			 * WITHOUT this check, we'd generate:
			 * - Path using 1 first:  [1, 1', 2], [1, 2, 1']
			 * - Path using 1' first: [1', 1, 2], [1', 2, 1]  <- DUPLICATES!
			 * 
			 * WITH this check:
			 * - When at 1' (i=1), if 1 (i=0) is NOT used yet, we skip 1'
			 * - This forces: use 1 before 1', preventing duplicate permutations
			 * - Valid paths: [1, 1', 2], [1, 2, 1'], [2, 1, 1'] (unique results)
			 * 
			 * Another example: nums = [2, 2, 2]
			 * - Must use nums[0] before nums[1], and nums[1] before nums[2]
			 * - Only generates: [2, 2, 2] (one unique permutation)
			 */
			if (i>0 && nums[i] == nums[i - 1] && !used[i - 1]) continue;
			
			used[i] = true;
			path.push_back(nums[i]);
			dfs(nums, used, path, result);
			path.pop_back();
			used[i] = false;
		}
	
	}

};