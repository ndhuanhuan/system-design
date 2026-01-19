// https://leetcode.com/problems/best-meeting-point/description/
class Solution {
public:
    int minTotalDistance(vector<vector<int>>& grid) {
        vector<int> r, c;
        
        for (int i = 0; i < grid.size(); ++i) {
            for (int j = 0; j < grid[0].size(); ++j) {
                if(grid[i][j]) {
                    r.push_back(i);
                }
            }
        }
        
       for (int j = 0; j < grid[0].size(); ++j) {
            for (int i = 0; i < grid.size(); ++i) {
                if(grid[i][j]) {
                    c.push_back(j);
                }
            }
        }
        
        return helper(r) + helper(c);
    }
    
    int helper(vector<int>& v) {
        int res = 0;
        //sort(v.begin(), v.end());
        
        int  i = 0, j = v.size() - 1;
        
        while(i < j) res += (v[j--] - v[i++]);
        
        return res;
    }
};