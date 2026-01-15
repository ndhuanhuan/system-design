// https://leetcode.com/problems/random-pick-with-weight/description/
class Solution {
public:
    Solution(vector<int>& w) {
        sums_ = std::move(w);
        for (int i = 1; i < sums_.size(); ++i) {
            sums_[i] += sums_[i-1];
        }
    }
    
    int pickIndex() {
        float randNum = (float) rand() / RAND_MAX;
        float target = randNum * sums_.back();
        return upper_bound(sums_.begin(), sums_.end(), target) - sums_.begin();
    }
private:
    vector<int> sums_;
};

/**
 * Your Solution object will be instantiated and called as such:
 * Solution* obj = new Solution(w);
 * int param_1 = obj->pickIndex();
 */