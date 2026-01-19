// https://leetcode.com/problems/lowest-common-ancestor-of-a-binary-tree-iii/description/
/*
// Definition for a Node.
class Node {
public:
    int val;
    Node* left;
    Node* right;
    Node* parent;
};
*/

class Solution {
public:
    Node* lowestCommonAncestor(Node* p, Node * q) {
        Node *pp = p, *qq = q;

        while(pp != qq) {
            pp = pp->parent ? pp->parent : q;
            qq = qq->parent ? qq->parent: p;
        }

        return pp;
    }
};