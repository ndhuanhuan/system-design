// https://leetcode.com/problems/insert-delete-getrandom-o1-duplicates-allowed/description/
// 381. Insert Delete GetRandom O(1) - Duplicates allowed
class RandomizedCollection {
public:
    RandomizedCollection() {
        
    }
    
    bool insert(int val) {
        m_[val].push_back(vals_.size());
        vals_.push_back({val, m_[val].size() -1});
        return m_[val].size() == 1u;
    }
    
    bool remove(int val) {
        if (!m_.count(val)) return false;
        int index_to_evict = m_[val].back();
        const auto& last_entry = vals_.back();
        m_[last_entry.first][last_entry.second] = index_to_evict;

        swap(vals_.back(), vals_[index_to_evict]);

        vals_.pop_back();
        m_[val].pop_back();
        if (m_[val].empty()) m_.erase(val);
        return true;
    }
    
    int getRandom() {
        return vals_[rand()%vals_.size()].first;
    }
private:
    // val -> indices array: indices in vals_
    unordered_map<int, vector<int>> m_;
    // {val, index in the indices array}
    vector<pair<int,int>> vals_;
};