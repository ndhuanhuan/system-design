// https://leetcode.com/problems/course-schedule-ii/
class Solution {
public:
    vector<int> findOrder(int numCourses, vector<vector<int>>& prerequisites) {
        vector<int> res;
        
        vector<vector<int>> g(numCourses, vector<int>(0));
        
        vector<int> in(numCourses, 0);
        
        for(auto& p: prerequisites) {
            g[p[1]].push_back(p[0]);
            in[p[0]]++;
        }
        
        queue<int> q;
        
        for(int i = 0; i < numCourses; ++i) {
            if(!in[i]) {
                q.push(i);
            }
        }
        
        while(!q.empty()) {
            int cur = q.front(); q.pop();
            res.push_back(cur);
            
            for(auto c: g[cur]) {
                in[c]--;
                if(in[c] == 0) {
                    q.push(c);
                }
            }
        }
        if(res.size() != numCourses) res.clear();
        return res;
    }
};