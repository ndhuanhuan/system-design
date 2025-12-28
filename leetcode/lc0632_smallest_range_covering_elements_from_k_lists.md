# Smallest Range Covering Elements from K Lists

## Problem Statement

You have `k` lists of sorted integers in non-decreasing order. Find the smallest range that includes at least one number from each of the k lists.

We define the range `[a, b]` is smaller than range `[c, d]` if:
- `b - a < d - c`, OR
- `a < c` if `b - a == d - c` (tie-breaker: prefer smaller starting point)

### Example 1:
```
Input: nums = [[4,10,15,24,26],[0,9,12,20],[5,18,22,30]]
Output: [20,24]

Explanation: 
List 1: [4, 10, 15, 24, 26], 24 is in range [20,24]
List 2: [0, 9, 12, 20], 20 is in range [20,24]
List 3: [5, 18, 22, 30], 22 is in range [20,24]
```

### Example 2:
```
Input: nums = [[1,2,3],[1,2,3],[1,2,3]]
Output: [1,1]
```

---

## Solution Approach: Min-Heap + Sliding Window

### Core Idea

The key insight is that **any valid range must be bounded by the minimum and maximum elements currently being considered from each list**.

To find the smallest range:
1. Start by considering the first element from each list
2. The current range is `[min_element, max_element]`
3. To potentially shrink the range, advance the pointer of the list that contains the minimum element
4. Update the range if we find a smaller one
5. Continue until we can't advance anymore (one list is exhausted)

### Why This Works

- We must include at least one element from each list
- The range is defined by the minimum and maximum of the selected elements
- By always advancing the list with the minimum element, we're trying to increase the minimum (potentially shrinking the range)
- We stop when any list is exhausted because we can no longer maintain coverage of all k lists

---

## Code Explanation

```cpp
class Solution {
public:
    vector<int> smallestRange(vector<vector<int>>& nums) {
        int curMax = INT_MIN, n = nums.size();
        vector<int> idx(n, 0);  // Track current index for each list

        // Min-heap: stores {value, list_index}
        auto cmp = [](pair<int, int>& a, pair<int, int>& b) { 
            return a.first > b.first;
        };
        priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(cmp)> q(cmp);

        // Initialize: add first element from each list
        for (int i = 0; i < n; ++i) {
            q.push({nums[i][0], i});
            idx[i] = 1;  // Next element to consider is at index 1
            curMax = max(curMax, nums[i][0]);
        }

        // Initial range: [min element, max element]
        vector<int> res{q.top().first, curMax};

        // Continue while we can advance the list with minimum element
        while (idx[q.top().second] < nums[q.top().second].size()) {
            int t = q.top().second;  // List index with minimum element
            q.pop();
            
            // Add next element from that list
            q.push({nums[t][idx[t]], t});
            curMax = max(curMax, nums[t][idx[t]]);
            ++idx[t];

            // Update result if we found a smaller range
            if (res[1] - res[0] > curMax - q.top().first) {
                res = {q.top().first, curMax};
            }
        }

        return res;
    }
};
```

---

## Step-by-Step Walkthrough

### Example: `nums = [[4,10,15,24,26], [0,9,12,20], [5,18,22,30]]`

**Initialization:**
- Heap: `{(4, 0), (0, 1), (5, 2)}`  → min-heap, so front is (0, 1)
- curMax: `5`
- idx: `[1, 1, 1]`
- res: `[0, 5]` (range size = 5)

**Iteration 1:**
- Min element: `0` from list 1
- Pop `(0, 1)`, push next from list 1: `(9, 1)`
- Heap: `{(4, 0), (9, 1), (5, 2)}` → front is (4, 0)
- curMax: `max(5, 9) = 9`
- New range: `[4, 9]` (size = 5, same but better start)
- res: `[4, 9]`

**Iteration 2:**
- Min element: `4` from list 0
- Pop `(4, 0)`, push `(10, 0)`
- Heap: `{(5, 2), (9, 1), (10, 0)}` → front is (5, 2)
- curMax: `max(9, 10) = 10`
- New range: `[5, 10]` (size = 5, worse start)
- res: `[4, 9]` (keep old)

**Iteration 3:**
- Min element: `5` from list 2
- Pop `(5, 2)`, push `(18, 2)`
- Heap: `{(9, 1), (10, 0), (18, 2)}` → front is (9, 1)
- curMax: `max(10, 18) = 18`
- New range: `[9, 18]` (size = 9, worse)
- res: `[4, 9]` (keep old)

... *continuing this process* ...

**Eventually arrives at:**
- Heap: `{(20, 1), (24, 0), (22, 2)}`
- curMax: `24`
- Range: `[20, 24]` (size = 4)
- res: `[20, 24]` ✓

The algorithm stops when advancing the minimum element would exhaust one list.

---

## Complexity Analysis

### Time Complexity: **O(n × k × log k)**
- `n` = average length of each list
- `k` = number of lists
- We process at most `n × k` elements total
- Each heap operation (push/pop) takes `O(log k)` since heap size is `k`

### Space Complexity: **O(k)**
- Heap stores at most `k` elements
- `idx` array stores `k` indices

---

## Key Takeaways

1. **Min-Heap for tracking minimum**: Efficiently find the smallest element across all lists in O(log k) time
2. **Greedy advancement**: Always advance the list with the minimum element to try to shrink the range
3. **Termination condition**: Stop when any list is exhausted (can't cover all k lists anymore)
4. **Track maximum separately**: While heap gives us minimum, we maintain `curMax` to define the upper bound

This problem elegantly combines:
- Merge K sorted lists concept (min-heap)
- Sliding window technique (advancing pointers)
- Greedy optimization (always move the minimum)
