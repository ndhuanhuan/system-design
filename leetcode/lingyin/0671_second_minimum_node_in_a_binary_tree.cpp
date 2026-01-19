// https://leetcode.com/problems/second-minimum-node-in-a-binary-tree/description/
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
class Solution {
public:
    int findSecondMinimumValue(TreeNode* root) {
        if(!root) return -1;
        deque<TreeNode*> q;
        
        q.push_back(root);
        
        int s1 = root->val;
        int s2 = -1;
        bool found = false;
        
        while(!q.empty()) {
            auto cur = q.front();
            q.pop_front();
            
            if(cur->val > s1 && (s2 == -1 || cur->val < s2)) {
                found = true;
                s2 = cur->val;
                continue;
            }
            
            if(cur->left) q.push_back(cur->left);
            if(cur->right) q.push_back(cur->right);
        }
        
        return s2;
    }
};