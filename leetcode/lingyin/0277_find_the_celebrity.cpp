// https://leetcode.com/problems/find-the-celebrity/description/
/* The knows API is defined for you.
      bool knows(int a, int b); */

class Solution {
public:
    int findCelebrity(int n) {
        if (n <= 1) return n;
        int candidate = 0;

        for (int i = 1; i < n; ++i) {
            if(!knows(i, candidate)) {
                candidate = i;
            }
        }

        for (int i = 0; i < n; ++i) {
            if (i != candidate) {
                if (!knows(i, candidate) || knows(candidate, i)) {
                    return -1;
                }
            }
        }
        return candidate;
    }
};