// https://leetcode.com/problems/max-stack/
class MaxStack {
public:
    MaxStack() {
        
    }
    
    void push(int x) {
        v.push_front(x);
        mp[x].push_back(v.begin());
    }
    
    int pop() {
        int x = *v.begin();
        mp[x].pop_back();
        if(mp[x].empty()) mp.erase(x);
        v.erase(v.begin());
        return x;
    }
    
    int top() {
        return *v.begin();
    }
    
    int peekMax() {
        return mp.begin()->first;
    }
    
    int popMax() {
        int x = peekMax();
        auto it = mp[x].back();
        mp[x].pop_back();
        if(mp[x].empty()) mp.erase(x);
        v.erase(it);
        return x;
    }
private:
    list<int> v;
    map<int, vector<list<int>::iterator>, greater<int>> mp;
};

/**
 * Your MaxStack object will be instantiated and called as such:
 * MaxStack* obj = new MaxStack();
 * obj->push(x);
 * int param_2 = obj->pop();
 * int param_3 = obj->top();
 * int param_4 = obj->peekMax();
 * int param_5 = obj->popMax();
 */