// https://leetcode.com/problems/lowest-common-ancestor-of-a-binary-search-tree/
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
// https://leetcode.cn/problems/lowest-common-ancestor-of-a-binary-search-tree/solutions/2023873/zui-jin-gong-gong-zu-xian-yi-ge-shi-pin-8h2zc/?envType=company&envId=linkedin&favoriteSlug=linkedin-thirty-days
class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        int cur = root->val;
        if (p->val < cur && q->val < cur) {
            return lowestCommonAncestor(root->left, p, q);
        } else if (p->val > cur && q->val > cur) {
            return lowestCommonAncestor(root->right, p, q);
        }

        return root;
    }
};