// https://leetcode.com/problems/graph-valid-tree/description/
class DSU {
public:
    DSU(int n): p_(n) {
        for (int i = 0; i < n; ++i) {
            p_[i] = i;
        }
    }

    void Union(int a, int b) {
        p_[Find(a)] = Find(b);
    }

    int Find(int a) {
        if (p_[a] != a) p_[a] = Find(p_[a]);
        return p_[a];
    }

private:
    vector<int> p_;
};


class Solution {
public:
    bool validTree(int n, vector<vector<int>>& edges) {
        DSU dsu(n);

        for (auto& edge: edges) {
            int a = dsu.Find(edge[0]);
            int b = dsu.Find(edge[1]);
            if (a == b) return false;
            dsu.Union(edge[0], edge[1]);
        }

        return edges.size() == n - 1;
    }
};