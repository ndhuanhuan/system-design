// https://leetcode.com/problems/binary-tree-upside-down/description/

/*
BINARY TREE UPSIDE DOWN - VISUAL TRANSFORMATION GUIDE

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
WHAT IS "UPSIDE DOWN"?
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

Original Tree:          After Upside Down:
      1                        4
     / \                      / \
    2   3                    5   2
   / \                          / \
  4   5                        3   1

Transformation Rules (applied level by level, bottom to top):
1. Original LEFT child → becomes new ROOT
2. Original ROOT → becomes new RIGHT child  
3. Original RIGHT sibling → becomes new LEFT child

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
STEP-BY-STEP VISUALIZATION
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

Start with:     1
               / \
              2   3
             / \
            4   5

Process node 2 (bottom-up):
- 2's left child (4) becomes parent of 2
- 2 becomes right child of 4
- 2's right child (5) becomes left child of 4

After processing 2:
        4
       / \
      5   2
         / \
        ?   ?

Process node 1 (continue bottom-up):
- 1's left child (2) is already processed
- 2 becomes parent of 1
- 1 becomes right child of 2
- 1's right child (3) becomes left child of 2

Final result:
        4
       / \
      5   2
         / \
        3   1

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
KEY INSIGHT: Linked List Reversal Pattern!
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

If you follow the left spine (1→2→4), it's like reversing a linked list!
- We traverse down the left spine
- We reverse pointers as we go
- We also need to track the right sibling at each level

Think of it as reversing this "linked list":
1 → 2 → 4 → NULL    (left spine)
↓   ↓   ↓
3   5   NULL        (right siblings)

We need to track THREE things at each step:
- node: Current node being processed
- parent: Previous node (will become current node's right child)
- right: Parent's right sibling (will become current node's left child)

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
ALGORITHM WALKTHROUGH (with example)
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

Initial:  node=1, parent=NULL, right=NULL

Iteration 1: node=1
  Save next = 1.left = 2
  1.left = right (NULL)      → 1.left = NULL
  Save right = 1.right = 3    → right = 3 for next iteration
  1.right = parent (NULL)     → 1.right = NULL
  parent = 1                  → parent = 1 for next iteration
  node = next (2)             → move to node 2

Iteration 2: node=2
  Save next = 2.left = 4
  2.left = right (3)          → 2.left = 3
  Save right = 2.right = 5    → right = 5 for next iteration
  2.right = parent (1)        → 2.right = 1
  parent = 2                  → parent = 2 for next iteration
  node = next (4)             → move to node 4

Iteration 3: node=4
  Save next = 4.left = NULL
  4.left = right (5)          → 4.left = 5
  Save right = 4.right = NULL → right = NULL for next iteration
  4.right = parent (2)        → 4.right = 2
  parent = 4                  → parent = 4 for next iteration
  node = next (NULL)          → exit loop

Return parent (4) → This is the new root!

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
MEMORY TRICK
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
"PLR" - Parent becomes Left, Right stays Right (but moves down)
At each node:
- P: Parent moves to current.right
- L: Left (saved for next iteration)  
- R: Right sibling moves to current.left

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
INTERVIEW TIP
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Draw the transformation step-by-step on whiteboard. The visual makes it
obvious that we're reversing the left spine while carrying right siblings.
*/

public:
    TreeNode* upsideDownBinaryTree(TreeNode* root) {
        // Three pointers for the "linked list reversal" pattern
        TreeNode* node = root;      // Current node being processed
        TreeNode* parent = NULL;    // Previous node (becomes current's right child)
        TreeNode* right = NULL;     // Parent's right sibling (becomes current's left child)

        // Traverse down the left spine (like traversing a linked list)
        while (node != NULL) {
            // Step 1: Save next node to process (current's left child)
            TreeNode* next = node->left;
            
            // Step 2: Rewire current node's left pointer
            // Current's left = parent's right sibling (saved from previous iteration)
            node->left = right;
            
            // Step 3: Save current's right child for next iteration
            // (It will become the next node's left child)
            right = node->right;
            
            // Step 4: Rewire current node's right pointer
            // Current's right = parent (reversing the connection)
            node->right = parent;
            
            // Step 5: Move pointers forward for next iteration
            parent = node;  // Current node becomes parent for next iteration
            node = next;    // Move to next node (down the left spine)
        }

        // When loop ends: node is NULL, parent is the new root (leftmost node)
        return parent;
    }
};