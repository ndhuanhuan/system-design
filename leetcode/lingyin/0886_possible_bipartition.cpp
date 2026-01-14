// https://leetcode.com/problems/possible-bipartition/description/
class Solution {
public:
    bool possibleBipartition(int n, vector<vector<int>>& dislikes) {
        g_ = vector<vector<int>>(n);
        for (auto& d: dislikes) {
            g_[d[0]-1].push_back(d[1]-1);
            g_[d[1]-1].push_back(d[0]-1);
        }

        colors_ = vector<int>(n, 0);

        for (int i = 0; i < n; ++i) {
            if(colors_[i] == 0 && !dfs(i, 1)) return false;
        }

        return true;
    }

private:
    vector<vector<int>> g_;
    vector<int> colors_;

    bool dfs(int cur, int color) {
        colors_[cur] = color;
        for (auto next: g_[cur]) {
            if (colors_[next] == color) return false;
            if (colors_[next] == 0 && !dfs(next, -color)) return false;
        }

        return true;
    }
};