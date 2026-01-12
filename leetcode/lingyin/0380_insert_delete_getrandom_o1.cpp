// https://leetcode.com/problems/insert-delete-getrandom-o1/description/
class RandomizedSet {
public:
    RandomizedSet() {
        
    }
    
    bool insert(int val) {
        if (m.count(val)) return false;
        m[val] = v.size();
        v.push_back(val);
        return true;
    }
    
    bool remove(int val) {
        if (!m.count(val)) return false;
        int index = m[val];
        int lastElem = v.back();
        int lastIndex = m[lastElem];
        v[index] = lastElem;
        m[lastElem] = index;
        v.pop_back();
        m.erase(val);

        return true;
    }
    
    int getRandom() {
        return v[rand()%v.size()];
    }
private:
    vector<int> v;
    unordered_map<int, int> m;
};

/**
 * Your RandomizedSet object will be instantiated and called as such:
 * RandomizedSet* obj = new RandomizedSet();
 * bool param_1 = obj->insert(val);
 * bool param_2 = obj->remove(val);
 * int param_3 = obj->getRandom();
 */