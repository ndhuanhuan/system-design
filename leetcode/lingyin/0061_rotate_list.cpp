// https://leetcode.com/problems/rotate-list/description/
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    ListNode* rotateRight(ListNode* head, int k) {
        if (!head) return head;

        ListNode* cur = head;
        int n = 1;
        while(cur->next) {
            cur = cur->next;
            n++;
        }

        int m = n - k%n;
        cur->next = head;

        cur = head;

        for (int i = 0;i < m-1; ++i) {
            cur = cur->next;
        }

        ListNode* newHead = cur->next;
        cur->next = nullptr;

        return newHead;
    }
};