// https://leetcode.com/problems/lru-cache/description/

// Thread-safe LRU Cache using shared_mutex for better read concurrency
// 
// Production-Grade Improvements (mention in interview if asked):
// 1. SHARDING: Split cache into N shards (e.g., key % N), each with own lock
//    - Reduces lock contention by N times
//    - Near-linear scaling with thread count
//
// 2. LOCK-FREE: Use atomic operations and CAS for hot paths
//    - Libraries: Intel TBB concurrent_hash_map, Folly EvictingCacheMap
//    - Trade-off: More complex, but best performance
//
// 3. APPROXIMATE LRU: Relax strict LRU for better parallelism
//    - CLOCK algorithm (single atomic counter)
//    - Random sampling (pick K random items, evict oldest)
//    - W-TinyLFU (used by Caffeine cache)
//
// 4. METRICS: Track hit rate, evictions, contention
//    - Use atomic counters for low overhead
//    - Monitor lock wait time
//
// 5. MEMORY MANAGEMENT: Custom allocator for list nodes
//    - Pool allocator to reduce malloc/free overhead
//    - Object pooling for high churn scenarios

#include <list>
#include <unordered_map>
#include <shared_mutex>
using namespace std;

class LRUCache {
public:
    LRUCache(int capacity) : capacity_(capacity) {}
    
    int get(int key) {
        // Shared lock: Multiple threads can read concurrently
        // BUT: We need exclusive lock because splice modifies the list!
        // This is a known LRU trade-off: even reads modify access order
        unique_lock<shared_mutex> lc(mt_);
        const auto it = m_.find(key);
        
        if(it == m_.cend()) return -1;
        
        // Move accessed item to front (most recently used)
        // splice() efficiently moves the node to the front in O(1) time
        // - cache_.begin(): Target position (front of the list)
        // - cache_: Source list (same list here)
        // - it->second: Iterator pointing to the node to move
        cache_.splice(cache_.begin(), cache_, it->second);
        
        // C++ std::list::splice documentation:
        // splice() moves elements from one part of the list to another in O(1) time.
        // Syntax: list.splice(position, source_list, iterator)
        // - position: Where to move the element(s) to
        // - source_list: The list containing the element(s) (can be the same list)
        // - iterator: The specific element to move
        // In this case, it moves the accessed node to the front of the list.
        
        return it->second->second;
    }
    
    void put(int key, int value) {
        unique_lock<shared_mutex> lc(mt_);  // Exclusive lock for writes
        const auto it = m_.find(key);
        
        // Key exists: update value and move to front
        if(it != m_.cend()) {
            it->second->second = value;
            cache_.splice(cache_.begin(), cache_, it->second);
            return;
        }
        
        // Evict LRU item if at capacity
        if(cache_.size() == capacity_) {
            const auto& node = cache_.back();
            m_.erase(node.first);
            cache_.pop_back();
        }
        
        // Insert new item at front
        cache_.emplace_front(key, value);
        m_[key] = cache_.begin();
    }

private:
    int capacity_;
    list<pair<int, int>> cache_;  // Doubly-linked list: (key, value)
    unordered_map<int, list<pair<int, int>>::iterator> m_;  // key -> list iterator
    mutable shared_mutex mt_;  // C++17: allows shared (read) and exclusive (write) locks
};

// Note: In true read-heavy scenarios, consider a read-through cache that tracks
// access order separately (e.g., separate atomic access counter) to allow
// true shared reads, with periodic background thread updating eviction policy.

/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache* obj = new LRUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */