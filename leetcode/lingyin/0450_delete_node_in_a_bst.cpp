// https://leetcode.com/problems/delete-node-in-a-bst/description/
// https://zxi.mytechroad.com/blog/tree/leetcode-450-delete-node-in-a-bst/

/*
BST DELETION - COMPLETE UNDERSTANDING GUIDE

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CORE INSIGHT: Deleting from BST has 3 cases based on children count
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

STEP 1: FIND THE NODE (standard BST search)
    if (key > root->val) → search right subtree
    if (key < root->val) → search left subtree
    if (key == root->val) → found it! Now handle deletion...

STEP 2: HANDLE 3 DELETION CASES

┌─────────────────────────────────────────────────────────────────┐
│ CASE 1: Node is a LEAF (no children)                           │
│         Just delete it and return null                          │
│                                                                 │
│    5          delete(3)         5                              │
│   / \         ────────→         /                              │
│  3   7                         null                            │
└─────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────┐
│ CASE 2: Node has ONE child                                     │
│         Replace node with its child                            │
│                                                                 │
│    5          delete(3)         5                              │
│   / \         ────────→         / \                            │
│  3   7                         2   7                           │
│ /                                                              │
│2                                                               │
│                                                                │
│ WHY? The child already satisfies BST property relative to     │
│      the parent, so just "pull it up"                         │
└─────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────┐
│ CASE 3: Node has TWO children ★ THE TRICKY ONE ★              │
│                                                                 │
│    5          delete(5)         6                              │
│   / \         ────────→         / \                            │
│  3   7                         3   7                           │
│     / \                               \                        │
│    6   8                               8                       │
│                                                                │
│ SOLUTION: Replace with SUCCESSOR (or predecessor)             │
│                                                                │
│ 1. Find successor = smallest node in RIGHT subtree            │
│    (go right once, then keep going left)                      │
│                                                                │
│ 2. Copy successor's value to current node                     │
│                                                                │
│ 3. Recursively delete successor from right subtree            │
│    (successor has at most ONE child, so it's Case 1 or 2!)    │
│                                                                │
│ WHY SUCCESSOR WORKS?                                           │
│ - Successor is the NEXT larger value                          │
│ - It's larger than entire left subtree (we're in right tree)  │
│ - It's smaller than rest of right subtree (it's the smallest) │
│ - Perfect replacement that maintains BST property!            │
└─────────────────────────────────────────────────────────────────┘

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
KEY MENTAL MODEL: "Pull up" strategy
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
- 0 children: Pull up nothing (null)
- 1 child: Pull up that child
- 2 children: Pull up successor (which has ≤1 child, recursive!)

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
ALTERNATIVE: Could use PREDECESSOR instead
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Predecessor = largest in LEFT subtree (go left, then keep right)
Both work! Choice doesn't matter for correctness.

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
INTERVIEW TIP: Explain the 3 cases upfront before coding!
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
*/

class Solution {
public:
  TreeNode* deleteNode(TreeNode* root, int key) {
    // Base case: empty tree
    if (!root) return root;
    
    // STEP 1: Find the node to delete (standard BST search)
    if (key > root->val) {
        // Target is in right subtree
        root->right = deleteNode(root->right, key);
    } else if (key < root->val) {
        // Target is in left subtree
        root->left = deleteNode(root->left, key);
    } else {
        // FOUND IT! Now handle deletion based on number of children
        
        // CASE 3: Node has TWO children
        // Strategy: Replace with successor, then delete successor
        if (root->left != nullptr && root->right != nullptr) {
            // Find successor = smallest in right subtree
            // (go right once, then keep going left)
            TreeNode* min = root->right;
            while(min->left != nullptr) {
                min = min->left;
            }
            
            // Replace current node's value with successor's value
            root->val = min->val;
            
            // Delete the successor from right subtree
            // (successor has at most 1 child, so recursion handles Case 1 or 2)
            root->right = deleteNode(root->right, min->val);

        } else {
            // CASE 1 & 2: Node has 0 or 1 child
            // Just replace current node with its child (or null if no child)
            TreeNode* new_root = root->left == nullptr ? root->right : root->left;
            
            // Clean up: prevent dangling pointers before deletion
            root->left = root->right = nullptr;
            delete root;
            
            return new_root;
        }
    }
    
    return root;
  }
};