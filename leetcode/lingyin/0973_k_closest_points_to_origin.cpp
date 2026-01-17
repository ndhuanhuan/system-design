// https://leetcode.com/problems/k-closest-points-to-origin/description/
class Solution {
public:
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

    struct Comp {
      bool operator()(vector<int>& v1, vector<int>& v2) {
          return v1[0] * v1[0] + v1[1] * v1[1] < v2[0] * v2[0] + v2[1] * v2[1];
      }
    };
};