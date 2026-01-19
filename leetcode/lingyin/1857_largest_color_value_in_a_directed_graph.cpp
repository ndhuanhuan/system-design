// https://leetcode.com/problems/largest-color-value-in-a-directed-graph/
class Solution {
public:
    int largestPathValue(string colors, vector<vector<int>>& edges) {
        int n = colors.size();
        
        // Build adjacency list and calculate indegrees
        unordered_map<int, vector<int>> g;
        vector<int> deg(n);
        for (const auto& e : edges) {
            g[e[0]].push_back(e[1]);
            deg[e[1]]++;
        }
        
        // cnt[i][j] = max count of j-th color in any path ending at node i
        vector<vector<int>> cnt(n, vector<int>(26, 0));
        
        // Initialize queue with source nodes (indegree == 0)
        queue<int> q;
        for (int i = 0; i < n; ++i) {
            if (deg[i] == 0) {
                q.push(i);
                cnt[i][colors[i] - 'a'] = 1;
            }
        }
        
        int ans = 0, seen = 0;
        
        // Topological sort with DP
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            
            // Node u's color is already counted in cnt[u]:
            // - Source nodes: initialized when added to queue (line 22)
            // - Non-source nodes: counted when updated by predecessors (line 42)
            ans = max(ans, *max_element(cnt[u].begin(), cnt[u].end()));
            seen++;
            
            // Update all neighbors
            for (int v : g[u]) {
                for (int i = 0; i < 26; ++i) {
                    // Propagate color counts, incrementing if neighbor has this color
                    cnt[v][i] = max(cnt[v][i], cnt[u][i] + (i == colors[v] - 'a'));
                }
                
                // Add neighbor to queue if all predecessors processed
                if (--deg[v] == 0) {
                    q.push(v);
                }
            }
        }
        
        // If not all nodes processed, there's a cycle
        return seen < n ? -1 : ans;
    }
};