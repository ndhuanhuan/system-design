// # Merge N-ary Tree
// (This question is a variation of the LeetCode question 617. Merge Two Binary Trees. If you haven't completed that question yet, it is recommended to solve it first.)

// Implement a function `mergeTrees(TreeNode root1, TreeNode root2)` that takes two n-ary trees and returns a new n-ary tree formed by merging them node by node. Each node contains an integer value and a list of children; children are aligned by their index in the `children` list. The merge rules are:

// * If both trees have a node at the same position (0-based children array), the merged node's value is the sum of the two original values.
// * If only one tree has a node at a position, the merged tree should include a copy of that entire subtree.

// The definition of an n-ary tree node is below:

// ```java
// class Node {
//     int val;
//     List<Node> children;
//     ...
// }
// ```

// Return the root of the newly merged tree.

// Example 1:

// Input:

// root1:

// ```
//    1
//  / | \
// 2  3  5
// ```

// root2:

// ```
//   4
//   |
//   6
// ```

// Output:

// ```
//    5
//  / | \
// 8  3  5
// ```

// Explanation: The merged root has value 1 + 4 = 5. Its first child becomes 2 + 6 = 8, and its other children remain the same as the first tree.

// ![example1](./1.png)

// Example 2:

// Input:

// root1:

// ```
//   1
//  / \
// 2   3
// ```

// root2:

// ```
//   2
//  / \
// 1   4
// ```

// Output:

// ```
//   3
//  / \
// 3   7
// ```

// Example 3:

// Input:

// root1

// ```
//   1
//  / \
// 2   3
// |   |
// 4   5
//     |
//     7
// ```

// root2

// ```
//    2
//  / | \
// 1  4  9
// |  |
// 3  6
//    |
//    8
// ```

// Output:

// ```
//    3
//  / | \
// 3  7  9
// |  |
// 7  11
//    |
//    15
// ```

#include <iostream>
#include <vector>
#include <queue>
#include <cassert>
using namespace std;

// N-ary tree node definition
class Node {
public:
    int val;
    vector<Node*> children;
    
    Node() : val(0) {}
    Node(int _val) : val(_val) {}
    Node(int _val, vector<Node*> _children) : val(_val), children(_children) {}
};

class Solution {
public:
    // Merge two n-ary trees similar to merge binary trees
    // Key: Process children by index, merge matching positions
    Node* mergeTrees(Node* root1, Node* root2) {
        // Base cases: if either is null, return the other
        if (!root1 && !root2) return nullptr;
        if (!root1) return copyTree(root2);
        if (!root2) return copyTree(root1);
        
        // Both nodes exist: merge them
        Node* merged = new Node(root1->val + root2->val);
        
        // Merge children by index
        int maxChildren = max(root1->children.size(), root2->children.size());
        for (int i = 0; i < maxChildren; ++i) {
            Node* child1 = (i < root1->children.size()) ? root1->children[i] : nullptr;
            Node* child2 = (i < root2->children.size()) ? root2->children[i] : nullptr;
            
            Node* mergedChild = mergeTrees(child1, child2);
            if (mergedChild) {
                merged->children.push_back(mergedChild);
            }
        }
        
        return merged;
    }

private:
    // Helper: deep copy an entire subtree
    Node* copyTree(Node* root) {
        if (!root) return nullptr;
        
        Node* newNode = new Node(root->val);
        for (Node* child : root->children) {
            newNode->children.push_back(copyTree(child));
        }
        return newNode;
    }
};

// ==================== TEST UTILITIES ====================

// Helper: Create a node with given value and children
Node* createNode(int val, vector<Node*> children = {}) {
    return new Node(val, children);
}

// Helper: Print tree level by level for visualization
void printTree(Node* root) {
    if (!root) {
        cout << "null" << endl;
        return;
    }
    
    queue<Node*> q;
    q.push(root);
    
    while (!q.empty()) {
        int size = q.size();
        for (int i = 0; i < size; ++i) {
            Node* node = q.front();
            q.pop();
            
            cout << node->val << " [";
            for (int j = 0; j < node->children.size(); ++j) {
                cout << node->children[j]->val;
                if (j < node->children.size() - 1) cout << ",";
                q.push(node->children[j]);
            }
            cout << "] ";
        }
        cout << endl;
    }
}

// Helper: Verify tree structure matches expected values
bool verifyTree(Node* root, const vector<vector<int>>& expected) {
    if (!root && expected.empty()) return true;
    if (!root || expected.empty()) return false;
    
    queue<Node*> q;
    q.push(root);
    int level = 0;
    
    while (!q.empty() && level < expected.size()) {
        int size = q.size();
        if (size != expected[level].size()) return false;
        
        for (int i = 0; i < size; ++i) {
            Node* node = q.front();
            q.pop();
            
            if (node->val != expected[level][i]) return false;
            
            for (Node* child : node->children) {
                q.push(child);
            }
        }
        level++;
    }
    
    return level == expected.size() && q.empty();
}

// ==================== TEST CASES ====================

void testCase1() {
    cout << "\n=== Test Case 1 ===" << endl;
    // root1:    1          root2:    4
    //         / | \                  |
    //        2  3  5                 6
    
    Node* root1 = createNode(1, {
        createNode(2),
        createNode(3),
        createNode(5)
    });
    
    Node* root2 = createNode(4, {
        createNode(6)
    });
    
    Solution sol;
    Node* merged = sol.mergeTrees(root1, root2);
    
    cout << "Expected:    5" << endl;
    cout << "           / | \\" << endl;
    cout << "          8  3  5" << endl;
    cout << "\nActual:" << endl;
    printTree(merged);
    
    // Verify: level 0: [5], level 1: [8, 3, 5]
    assert(verifyTree(merged, {{5}, {8, 3, 5}}));
    cout << "✓ Test Case 1 Passed" << endl;
}

void testCase2() {
    cout << "\n=== Test Case 2 ===" << endl;
    // root1:  1        root2:  2
    //        / \              / \
    //       2   3            1   4
    
    Node* root1 = createNode(1, {
        createNode(2),
        createNode(3)
    });
    
    Node* root2 = createNode(2, {
        createNode(1),
        createNode(4)
    });
    
    Solution sol;
    Node* merged = sol.mergeTrees(root1, root2);
    
    cout << "Expected:  3" << endl;
    cout << "          / \\" << endl;
    cout << "         3   7" << endl;
    cout << "\nActual:" << endl;
    printTree(merged);
    
    assert(verifyTree(merged, {{3}, {3, 7}}));
    cout << "✓ Test Case 2 Passed" << endl;
}

void testCase3() {
    cout << "\n=== Test Case 3 ===" << endl;
    // root1:   1          root2:    2
    //         / \                 / | \
    //        2   3               1  4  9
    //        |   |               |  |
    //        4   5               3  6
    //            |                  |
    //            7                  8
    
    Node* root1 = createNode(1, {
        createNode(2, {createNode(4)}),
        createNode(3, {createNode(5, {createNode(7)})})
    });
    
    Node* root2 = createNode(2, {
        createNode(1, {createNode(3)}),
        createNode(4, {createNode(6, {createNode(8)})}),
        createNode(9)
    });
    
    Solution sol;
    Node* merged = sol.mergeTrees(root1, root2);
    
    cout << "Expected:    3" << endl;
    cout << "           / | \\" << endl;
    cout << "          3  7  9" << endl;
    cout << "          |  |" << endl;
    cout << "          7  11" << endl;
    cout << "             |" << endl;
    cout << "             15" << endl;
    cout << "\nActual:" << endl;
    printTree(merged);
    
    assert(verifyTree(merged, {{3}, {3, 7, 9}, {7, 11}, {15}}));
    cout << "✓ Test Case 3 Passed" << endl;
}

void testCase4() {
    cout << "\n=== Test Case 4: Both null ===" << endl;
    Solution sol;
    Node* merged = sol.mergeTrees(nullptr, nullptr);
    assert(merged == nullptr);
    cout << "✓ Test Case 4 Passed" << endl;
}

void testCase5() {
    cout << "\n=== Test Case 5: One null ===" << endl;
    Node* root1 = createNode(5, {createNode(1), createNode(2)});
    
    Solution sol;
    Node* merged = sol.mergeTrees(root1, nullptr);
    
    assert(verifyTree(merged, {{5}, {1, 2}}));
    cout << "✓ Test Case 5 Passed" << endl;
}

int main() {
    cout << "Running N-ary Tree Merge Tests..." << endl;
    
    testCase1();
    testCase2();
    testCase3();
    testCase4();
    testCase5();
    
    cout << "\n✓✓✓ All tests passed! ✓✓✓" << endl;
    return 0;
}