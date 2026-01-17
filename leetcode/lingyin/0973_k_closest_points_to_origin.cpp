// https://leetcode.com/problems/k-closest-points-to-origin/description/
class Solution {
public:
    // Approach 1: Using separate struct comparator
    vector<vector<int>> kClosest(vector<vector<int>>& points, int k) {
        priority_queue<vector<int>, vector<vector<int>>, Comp> pq;

        for (auto& p: points) {
            pq.push(p);
            if (pq.size() > k) pq.pop();
        }

        vector<vector<int>> res;
        while(!pq.empty()) {
            res.push_back(pq.top());
            pq.pop();
        }

        return res;
    }

    // Approach 2: Inline lambda comparator
    vector<vector<int>> kClosest_Inline(vector<vector<int>>& points, int k) {
        // Lambda must be defined before pq declaration
        auto comp = [](const vector<int>& v1, const vector<int>& v2) {
            return v1[0] * v1[0] + v1[1] * v1[1] < v2[0] * v2[0] + v2[1] * v2[1];
        };
        // Use decltype to get lambda type, pass lambda instance to constructor
        priority_queue<vector<int>, vector<vector<int>>, decltype(comp)> pq(comp);

        for (auto& p: points) {
            pq.push(p);
            if (pq.size() > k) pq.pop();
        }

        vector<vector<int>> res;
        while(!pq.empty()) {
            res.push_back(pq.top());
            pq.pop();
        }

        return res;
    }

    struct Comp {
      bool operator()(vector<int>& v1, vector<int>& v2) {
          return v1[0] * v1[0] + v1[1] * v1[1] < v2[0] * v2[0] + v2[1] * v2[1];
      }
    };
};