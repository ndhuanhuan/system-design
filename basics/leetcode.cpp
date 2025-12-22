// Problem: Find minimum cost path from node 0 to node n-1, where you can exclude 
// one edge (the maximum weight edge) from the total cost.
//
// You are given a positive integer n and a 2D integer array edges, where edges[i] = [ui, vi, wi].
// There is a weighted connected simple undirected graph with n nodes labeled from 0 to n - 1. 
// Each [ui, vi, wi] in edges represents an edge between node ui and node vi with positive weight wi.
//
// The cost of a path is the sum of weights of the edges in the path, excluding the edge with 
// the maximum weight. If there are multiple edges in the path with the maximum weight, only 
// the first such edge is excluded.
//
// Return an integer representing the minimum cost of a path going from node 0 to node n - 1.

// Example 1:
// Input: n = 5, edges = [[0,1,2],[1,2,7],[2,3,7],[3,4,4]]
// Output: 13
// Explanation:
// There is only one path going from node 0 to node 4: 0 -> 1 -> 2 -> 3 -> 4.
// The edge weights on this path are 2, 7, 7, and 4.
// Excluding the first edge with maximum weight, which is 1 -> 2, the cost of this path is 2 + 7 + 4 = 13.

// Example 2:
// Input: n = 3, edges = [[0,1,1],[1,2,1],[0,2,50000]]
// Output: 0
// Explanation:
// There are two paths going from node 0 to node 2:
// 0 -> 1 -> 2
// The edge weights on this path are 1 and 1.
// Excluding the first edge with maximum weight, which is 0 -> 1, the cost of this path is 1.
// 0 -> 2
// The only edge weight on this path is 1.
// Excluding the first edge with maximum weight, which is 0 -> 2, the cost of this path is 0.
// The minimum cost is min(1, 0) = 0.

// APPROACH: Modified Dijkstra's algorithm with state tracking
// Key insight: Track TWO states for each node:
//   - dist[node][0]: min cost to reach node WITHOUT using the free edge yet
//   - dist[node][1]: min cost to reach node AFTER using the free edge
// At each step, we have two choices:
//   1. Pay for the current edge (normal traversal)
//   2. Use the current edge as our "free" edge (skip its cost)

class Solution {
public:
    long long minCostExcludingMax(int n, vector<vector<int>>& edges) {
        // STEP 1: Build adjacency list representation of the graph
        // g[u] = list of {neighbor, weight} pairs
        vector<vector<pair<int, int>>> g(n);
        for (auto& e : edges) {
            int u = e[0], v = e[1], w = e[2];
            g[u].push_back({v, w});  // Add edge u -> v
            g[v].push_back({u, w});  // Add edge v -> u (undirected graph)
        }

        // STEP 2: Initialize distance array with two states per node
        // Use LLONG_MAX/4 to avoid overflow when adding weights
        long long inf = LLONG_MAX / 4;
        // dist[node][0] = min cost without using free edge
        // dist[node][1] = min cost after using free edge
        vector<array<long long, 2>> dist(n, {inf, inf});
        dist[0][0] = 0;  // Start at node 0 with cost 0, free edge not used

        // STEP 3: Initialize priority queue (min-heap)
        // Each entry: {cost, node, used_flag}
        // used_flag: 0 = haven't used free edge yet, 1 = already used it
        priority_queue<array<long long, 3>, vector<array<long long, 3>>, greater<array<long long, 3>>> pq;
        pq.push({0, 0, 0});  // Start: cost=0, node=0, free_edge_not_used=0

        // STEP 4: Dijkstra's algorithm with state tracking
        while (!pq.empty()) {
            auto t = pq.top();
            pq.pop();
            long long cur = t[0];  // Current accumulated cost
            int u = t[1];           // Current node
            int used = t[2];        // Free edge status (0=not used, 1=used)

            // Skip if we've already found a better path to this state
            if (cur > dist[u][used]) {
                continue;
            }
            
            // EARLY EXIT: If we reached destination AND used our free edge, we're done!
            // This is guaranteed to be the minimum cost due to Dijkstra's greedy property
            if (u == n - 1 && used == 1) {
                return cur;
            }

            // STEP 5: Explore all neighbors and make two types of transitions
            for (auto [v, w] : g[u]) {
                // CHOICE 1: Pay for this edge (normal transition)
                // Add the edge weight to current cost
                long long nxt = cur + w;
                if (nxt < dist[v][used]) {
                    dist[v][used] = nxt;
                    pq.push({nxt, v, used});  // used status stays the same
                }

                // CHOICE 2: Use this edge as our "free" edge (skip its cost)
                // Only possible if we haven't used the free edge yet (used == 0)
                if (used == 0) {
                    nxt = cur;  // Don't add weight! This edge is free
                    if (nxt < dist[v][1]) {
                        dist[v][1] = nxt;
                        pq.push({nxt, v, 1});  // Mark free edge as used (1)
                    }
                }
            }
        }

        // Return minimum cost to reach destination with free edge used
        // Note: We must use the free edge, so we return dist[n-1][1]
        return dist[n - 1][1];
    }
};