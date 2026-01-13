// https://leetcode.com/problems/intersection-of-two-linked-lists/description/
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */

/*
 * KEY INSIGHT: Why checking "a ? a->next : headB" works but "a->next ? a->next : headB" doesn't
 * 
 * CORRECT APPROACH (a ? a->next : headB):
 * - Checks if pointer 'a' itself is null
 * - When pointer reaches the last node, it moves to null on next iteration
 * - Then it detects null and switches to the other list's head
 * - Both pointers visit null exactly once before switching
 * - This ensures both traverse the same total distance: lengthA + lengthB
 * 
 * INCORRECT APPROACH (a->next ? a->next : headB):
 * - Checks if the NEXT node is null (before reaching it)
 * - When at the last node, detects next is null and immediately switches
 * - Never actually visits the null state
 * - Works when there IS intersection (both eventually meet at intersection node)
 * - FAILS when NO intersection: creates infinite loop because both pointers keep
 *   switching but never become equal (they never both reach null simultaneously)
 * 
 * Example with NO intersection: A=[1,2], B=[3,4]
 * Correct: a and b both eventually become null → loop exits
 * Incorrect: a→1→2→3→4→1→2→3→4... (infinite loop, never both null)
 */

class Solution {
public:
    // Approach 1: Correct - checks pointer itself before moving
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        ListNode* a = headA, *b = headB;
        while (a != b) {
            a = a ? a->next : headB;  // Check 'a' itself, not 'a->next'
            b = b ? b->next: headA;   // Check 'b' itself, not 'b->next'
        }
        return a;  // Returns intersection node or null if no intersection
    }
    
    // Approach 2: Fixed version - explicitly handle null to avoid infinite loop
    ListNode *getIntersectionNode_Fixed(ListNode *headA, ListNode *headB) {
        ListNode* p1 = headA, *p2 = headB;
        bool p1Switched = false, p2Switched = false;
        
        while (p1 != p2) {
            // Move to next or switch lists
            if (p1->next) {
                p1 = p1->next;
            } else if (!p1Switched) {
                p1 = headB;
                p1Switched = true;
            } else {
                // Already switched once and reached end again → no intersection
                return nullptr;
            }
            
            if (p2->next) {
                p2 = p2->next;
            } else if (!p2Switched) {
                p2 = headA;
                p2Switched = true;
            } else {
                // Already switched once and reached end again → no intersection
                return nullptr;
            }
        }
        return p1;
    }
};