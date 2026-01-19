// https://leetcode.com/problems/find-k-pairs-with-smallest-sums/

// Key Insight: Think of this as a matrix where matrix[i][j] = nums1[i] + nums2[j]
// Each row is sorted (nums2 is sorted), and each column is sorted (nums1 is sorted)
// We need to find k smallest values from this matrix
//
// Strategy: Use min-heap with "smart expansion"
// 1. Start with first column: (0,0), (1,0), (2,0), ... up to min(k, m) pairs
// 2. Always extract the smallest sum from heap
// 3. When we extract (i, j), we only add (i, j+1) - move right in the same row
// 4. This avoids duplicates because each row is processed independently
//
// Analogy: Priority queue compares the "front element" of each row and picks the smallest.
// After picking from a row, we advance that row by inserting its next element.
// Each row represents all pairs with the same nums1[i]: (i,0), (i,1), (i,2), ...
//
// Example: nums1=[1,7,11], nums2=[2,4,6], k=3
// Initial heap: [(0,0):3, (1,0):9, (2,0):13]  (pairs with their sums)
// Step 1: Extract (0,0)=3, add (0,1)  → heap: [(0,1):5, (1,0):9, (2,0):13]
// Step 2: Extract (0,1)=5, add (0,2)  → heap: [(0,2):7, (1,0):9, (2,0):13]
// Step 3: Extract (0,2)=7, done (k=3)
// Result: [[1,2], [1,4], [1,6]]

class Solution {
public:
    vector<vector<int>> kSmallestPairs(vector<int>& nums1, vector<int>& nums2, int k) {
        // Min-heap comparator: compare by sum of pairs
        // Store indices (i, j) where i is index in nums1, j is index in nums2
        auto cmp = [&nums1, &nums2](const pair<int, int> & a, const pair<int, int> & b) {
            return nums1[a.first] + nums2[a.second] > nums1[b.first] + nums2[b.second];
        };

        int m = nums1.size();
        int n = nums2.size();
        vector<vector<int>> ans;   
        priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(cmp)> pq(cmp);
        
        // Initialize: Add first column (all nums1[i] paired with nums2[0])
        // Only add min(k, m) to optimize - we don't need more than k pairs
        for (int i = 0; i < min(k, m); i++) {
            pq.emplace(i, 0);
        }
        
        // Extract k smallest pairs
        while (k-- > 0 && !pq.empty()) {
            auto [x, y] = pq.top(); 
            pq.pop();
            ans.emplace_back(initializer_list<int>{nums1[x], nums2[y]});
            
            // Expand right: add next element in same row (same i, next j)
            // This ensures each pair is added exactly once
            if (y + 1 < n) {
                pq.emplace(x, y + 1);
            }
        }

        return ans;
    }
};
