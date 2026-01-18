// https://leetcode.com/problems/combination-sum/description/
class Solution {
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        vector<vector<int>> res;
        vector<int> curRes;
        helper(candidates, 0, target, curRes, res);
        return res;
    }
    
    void helper(vector<int>& candidates, int start, int target, vector<int>& curRes, vector<vector<int>>& res) {
        if(target < 0) return;
        else if(target == 0) { res.push_back(curRes); }
        else {
            for(int i = start; i < candidates.size(); ++i) {
                curRes.push_back(candidates[i]);
                helper(candidates, i, target - candidates[i], curRes, res);
                curRes.pop_back();
            }
        }
    }
};