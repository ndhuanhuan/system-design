
// https://leetcode.com/problems/number-of-islands/
// class UnionFind {
//     vector<int> id;
// public:
//     UnionFind(int N) : id(N) {
//         iota(begin(id), end(id), 0);
//     }
//     void connect(int x, int y) {
//         int a = find(x), b = find(y);
//         if (a == b) return;
//         id[a] = b;
//     }
//     int find(int x) {
//         return id[x] == x ? x : (id[x] = find(id[x]));
//     }
// };
// class Solution {
//     int M, N;
//     int key(int x, int y) { return x * N + y; }
//     int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};
// public:
//     int numIslands(vector<vector<char>>& A) {
//         if (A.empty() || A[0].empty()) return 0;
//         M = A.size(), N = A[0].size();
//         UnionFind uf(M * N);
//         for (int i = 0; i < M; ++i) {
//             for (int j = 0; j < N; ++j) {
//                 if (A[i][j] == '0') continue;
//                 for (auto &dir : dirs) {
//                     int x = i + dir[0], y = j + dir[1];
//                     if (x < 0 || y < 0 || x >= M || y >= N || A[x][y] == '0') continue;
//                     uf.connect(key(i, j), key(x, y));
//                 }
//             }
//         }
//         unordered_set<int> s;
//         for (int i = 0; i < M; ++i) {
//             for (int j = 0; j < N; ++j) {
//                 if (A[i][j] == '0') continue;
//                 s.insert(uf.find(key(i, j)));
//             }
//         }
//         return s.size();
//     }
// };

class Solution {
public:
    int numIslands(vector<vector<char>>& grid) {
        if (grid.empty()) return 0;
        int m = grid.size();
        int n = grid[0].size();
        int ans = 0;
        for (int x = 0; x < m; ++x)
            for (int y = 0; y < n; ++y) {
                ans += grid[x][y] - '0';
                dfs(grid, x, y, m, n);
            }
        return ans;                
    }   
private:
    void dfs(vector<vector<char>>& grid, int x, int y, int m, int n) {
        if (x < 0 || y < 0 || x >= m || y >= n || grid[x][y] == '0')
            return;
        grid[x][y] = '0';
        dfs(grid, x + 1, y, m, n);
        dfs(grid, x - 1, y, m, n);
        dfs(grid, x, y + 1, m, n);
        dfs(grid, x, y - 1, m, n);
    }
};