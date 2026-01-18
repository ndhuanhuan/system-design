// https://leetcode.com/problems/all-nodes-distance-k-in-binary-tree/description/
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    vector<int> distanceK(TreeNode* root, TreeNode* target, int k) {
        buildGraph(nullptr, root);
        
        unordered_set<TreeNode*> seen;
        
        vector<int> res;
        
        queue<TreeNode*> q;
        q.push(target);
        seen.insert(target);
        int dis = 0;
        
        while(!q.empty() && dis <= k) {
            int sz = q.size();
            for (int i = 0; i < sz; ++i) {
                auto cur = q.front(); q.pop();
                if (dis == k) {
                    res.push_back(cur->val);
                }
                
                for (auto child: g[cur]) {
                    if (seen.count(child)) continue;
                    q.push(child);
                    seen.insert(child);
                }
            }
            ++dis;
        } 
        return res;
    }
private:
    unordered_map<TreeNode*, vector<TreeNode*>> g;
    
    void buildGraph(TreeNode* parent, TreeNode* child) {
        if (parent) {
            g[parent].push_back(child);
            g[child].push_back(parent);
        }
        
        if (child->left) buildGraph(child, child->left);
        if (child->right) buildGraph(child, child->right);
    }
};