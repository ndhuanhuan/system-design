// https://github.com/monkey0211/AlgoInPython/blob/main/LinkedIn/LinkedIn_Find%20Member%20Connection%20Distance.py
// # graph = {"Bob":["Alice","John"],
// #          "Alice":["Bob","Frank", "Lucy"],
// #          "Frank":["Alice"],
// #          "John":["Bob","Jenny"],
// #          "Jenny":["John", "Lucy"],
// #          "Lucy":["Jenny", "Alice"]}
// # 给一个graph 都是member connections. 给出两个member: m1, m2. 找最短路

// # 起始点固定的情况可以考虑bi-direction BFS. (可以加速搜索 不能减小复杂度)
// # 一个从头搜, 一个从尾搜, 最后如果相交 就直接return steps

// # variance1: 
// # Calling getConnections(1) returns the set of IDs for users {0, 2, 3, 4, 5}. getConnections(6) returns the set of IDs for users {4, 5}.
// # Sample expected outputs:
// # getDistance(0,6) => 3
// # getDistance(1,5) => 1
// # Graph edges:
// # {
// #   0: [1,3],
// #   1: [0,2,3,4,5],
// #   2: [1],
// #   3: [0,1,5],
// #   4: [1,5,6],
// #   5: [1,3,4,6],
// #   6: [4,5]
// # }
// # Input / Output
// # Input: two integer IDs userA and userB representing nodes in the graph.
// # Output: the length of the shortest path between userA and userB.
// g++ -std=c++11 linkedin_02.cpp -o linkedin_02 ./linkedin_02

#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <queue>
using namespace std;

// Time: O(V + E), Space: O(V)
class Solution {
public:
    int getDistance(vector<pair<string, string>>& connections, string m1, string m2) {
        if (m1 == m2) return 0;
        
        // Build graph
        unordered_map<string, vector<string>> graph;
        for (auto& conn : connections) {
            graph[conn.first].push_back(conn.second);
            graph[conn.second].push_back(conn.first);
        }
        
        unordered_set<string> visited;
        unordered_set<string> q1({m1});
        unordered_set<string> q2({m2});
        visited.insert(m1);
        visited.insert(m2);
        
        int step = 0;
        
        while (!q1.empty() && !q2.empty()) {
            step++;
            
            if (q1.size() > q2.size()) {
                swap(q1, q2);
            }
            
            unordered_set<string> q;
            
            for (string node : q1) {
                for (const string& nei : graph[node]) {
                    if (q2.count(nei)) return step;
                    if (visited.count(nei)) continue;
                    q.insert(nei);
                    visited.insert(nei);
                }
            }
            
            swap(q1, q);
        }
        
        return -1;
    }
};

int main() {
    Solution sol;
    
    vector<pair<string, string>> connections = {
        {"Bob", "Alice"}, {"Bob", "John"},
        {"Alice", "Frank"}, {"Alice", "Lucy"},
        {"John", "Jenny"}, {"Jenny", "Lucy"}
    };
    
    cout << sol.getDistance(connections, "Bob", "Alice") << endl;    // 1
    cout << sol.getDistance(connections, "Bob", "Bob") << endl;      // 0
    cout << sol.getDistance(connections, "Bob", "Lucy") << endl;     // 2
    cout << sol.getDistance(connections, "Bob", "Jenny") << endl;    // 2
    
    vector<pair<string, string>> empty;
    cout << sol.getDistance(empty, "Bob", "Nobody") << endl;         // -1
    
    return 0;
}