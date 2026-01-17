// https://leetcode.com/problems/design-hashmap/
class MyHashMap {
public:
    MyHashMap() {
        m_data.resize(m_size);
    }
    
    void put(int key, int value) {
        auto& l = m_data[key%m_size];
        for (auto& val: l) {
            if(val.first == key) {
                val.second = value;
                return;
            }
        }

        l.emplace_back(key, value);
    }
    
    int get(int key) {
        auto& l = m_data[key%m_size];
        for (auto& val: l) {
            if (val.first == key) {
                return val.second;
            }
        }
        return -1;
    }
    
    void remove(int key) {
        auto& l = m_data[key%m_size];
        for(auto it=l.begin(); it!= l.end(); ++it) {
            if(it->first==key) { l.erase(it); return; }
        }
    }

private:
    vector<list<pair<int, int>>> m_data;
    int m_size = 10000;
};

/**
 * Your MyHashMap object will be instantiated and called as such:
 * MyHashMap* obj = new MyHashMap();
 * obj->put(key,value);
 * int param_2 = obj->get(key);
 * obj->remove(key);
 */