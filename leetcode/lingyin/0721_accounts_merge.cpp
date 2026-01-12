// https://leetcode.com/problems/accounts-merge/description/
class DSU {
public:
    DSU(int n): p_(n) {
        for (int i = 0; i < n; ++i) {
            p_[i] = i;
        }
    }

    void Union(int i, int j) {
        p_[Find(i)] = Find(j);
    }

    int Find(int i) {
        if (p_[i] != i) p_[i] = Find(p_[i]);
        return p_[i];
    }
private:
    vector<int> p_;
};

class Solution {
public:
    vector<vector<string>> accountsMerge(vector<vector<string>>& accounts) {
        const int n = accounts.size();
        DSU dsu(n);

        unordered_map<string, int> m; // email to index mapping

        for (int i = 0; i < n; ++i) {
            for (int j = 1; j < accounts[i].size(); ++j) {
                string curEmail = accounts[i][j];
                if (!m.count(curEmail)) {
                    m[curEmail] = i;
                } else {
                    dsu.Union(m[curEmail], i);
                }
            }
        }

        unordered_map<int, set<string>> idEmail;

        for (int i = 0; i < n; ++i) {
            int root = dsu.Find(i);
            for (int j = 1; j < accounts[i].size(); ++j) {

                idEmail[root].insert(accounts[i][j]);
            }
        }

        vector<vector<string>> res;

        for(auto& kv: idEmail) {
            int id = kv.first;
            vector<string> item({accounts[id][0]});
            for (auto& email: kv.second) {
                item.push_back(email);
            }

            res.push_back(item);
        }

        return res;
    }
};