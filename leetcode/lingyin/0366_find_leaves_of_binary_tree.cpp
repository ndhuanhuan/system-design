// https://leetcode.com/problems/find-leaves-of-binary-tree/
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
// https://github.com/phantomwolf/leetcode-problems/blob/master/Interviews/%E8%B0%B7%E6%AD%8C/Medium-N-aryTreeLeafNodeRemoval/README.md
class Solution {
public:
    vector<vector<int>> findLeaves(TreeNode* root) {
        vector<vector<int>> res;
        dfs(root, res);
        return res;
    }

    int dfs(TreeNode* root, vector<vector<int>>& res) {
        if(!root) return -1;
        int depth = 1 + max(dfs(root->left, res), dfs(root->right, res));
        if(depth >= res.size()) res.push_back({});
        res[depth].push_back(root->val);
        return depth;
    }
};