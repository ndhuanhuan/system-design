// https://leetcode.com/problems/best-meeting-point/description/

// Key Insight: Manhattan distance = |x1-x2| + |y1-y2| can be separated into x and y components
// For 1D: The optimal meeting point is the MEDIAN (minimizes sum of absolute distances)
// For 2D: Solve x and y independently, then add results
//
// Clever trick: Instead of sorting, collect coordinates in sorted order by scanning strategically
// - Scan row by row (top to bottom) → row indices are naturally sorted
// - Scan column by column (left to right) → column indices are naturally sorted
//
// Example: grid = [[1,0,0,0,1],
//                  [0,0,0,0,0],
//                  [0,0,1,0,0]]
// Row indices: [0, 0, 2] (already sorted from scanning top to bottom)
// Col indices: [0, 2, 4] (already sorted from scanning left to right)
// Median row = 0, Median col = 2, Meeting point = (0, 2)
// Total distance = |0-0| + |0-0| + |0-2| + |0-0| + |2-0| + |2-2| = 0 + 2 + 0 + 2 + 0 = 4

class Solution {
public:
    int minTotalDistance(vector<vector<int>>& grid) {
        vector<int> r, c;
        
        // Collect row indices (naturally sorted by scanning row by row)
        for (int i = 0; i < grid.size(); ++i) {
            for (int j = 0; j < grid[0].size(); ++j) {
                if(grid[i][j]) {
                    r.push_back(i);
                }
            }
        }
        
        // Collect column indices (naturally sorted by scanning column by column)
        for (int j = 0; j < grid[0].size(); ++j) {
            for (int i = 0; i < grid.size(); ++i) {
                if(grid[i][j]) {
                    c.push_back(j);
                }
            }
        }
        
        // Sum of distances in x-dimension + sum of distances in y-dimension
        return helper(r) + helper(c);
    }
    
    // Calculate sum of distances from all points to the median
    // Two-pointer approach: pair smallest with largest, add their distance
    // Works because median property: sum = (max-min) + (2nd_max - 2nd_min) + ...
    // No need to explicitly find median or sort (already sorted from collection)
    int helper(vector<int>& v) {
        int res = 0;
        int i = 0, j = v.size() - 1;
        
        // Each iteration: pair outermost points and add their distance
        while(i < j) res += (v[j--] - v[i++]);
        
        return res;
    }
};