// https://leetcode.com/problems/closest-leaf-in-a-binary-tree/description/
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
    int findClosestLeaf(TreeNode* root, int k) {
        graph_.clear();
        start_ = nullptr;
        buildGraph(root, nullptr, k);
        queue<TreeNode*> q;
        q.push(start_);
        unordered_set<TreeNode*> seen;
        while(!q.empty()) {
            auto cur = q.front(); q.pop();
            seen.insert(cur);
            if(!cur->left && !cur->right) return cur->val;
            for(auto& neighbor: graph_[cur]) {
                if (!seen.count(neighbor)) q.push(neighbor);
            }
        }
        
        return 0;
        
    }
    

    
private:
    void buildGraph(TreeNode* node, TreeNode* parent, int k) {
        if(!node) return;
        if(node->val == k) start_ = node;
        if(parent) {
            graph_[node].push_back(parent);
            graph_[parent].push_back(node);
        }
        
        buildGraph(node->left, node, k);
        buildGraph(node->right, node, k);
    }
    
    unordered_map<TreeNode*, vector<TreeNode*>> graph_;
    TreeNode* start_;
};