// https://leetcode.com/problems/closest-binary-search-tree-value-ii/
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
    vector<int> closestKValues(TreeNode* root, double target, int k) {
        vector<int> res;
        stack<TreeNode*> st;
        TreeNode* cur = root;

        while(cur || !st.empty()) {
            while(cur) {
                st.push(cur);
                cur = cur->left;
            }

            cur = st.top(); st.pop();

            if (res.size() < k) {
                res.push_back(cur->val);
            } else if (abs(cur->val - target) < abs(res[0] - target)) {
                res.push_back(cur->val);
                res.erase(res.begin());
            } else {
                break;
            }


            cur = cur->right;
        }

        return res;
    }
};