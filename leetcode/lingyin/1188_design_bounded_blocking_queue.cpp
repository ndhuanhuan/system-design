// https://leetcode.com/problems/design-bounded-blocking-queue/description/
// 1188. Design Bounded Blocking Queue

/*
BLOCKING QUEUE - PRODUCER/CONSUMER PATTERN EXPLAINED

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
WHAT IS A BLOCKING QUEUE?
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
A thread-safe queue where:
- enqueue() BLOCKS (waits) when queue is FULL
- dequeue() BLOCKS (waits) when queue is EMPTY

Use case: Producer-Consumer pattern
- Producer threads add items → blocked if full
- Consumer threads remove items → blocked if empty

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
THREE SYNCHRONIZATION PRIMITIVES NEEDED
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

1. MUTEX (mutual exclusion lock)
   - Ensures only ONE thread accesses queue at a time
   - Prevents race conditions (e.g., two threads pushing simultaneously)

2. CONDITION_VARIABLE (CV)
   - Allows threads to WAIT for a condition to become true
   - Allows threads to NOTIFY others when condition changes
   
   Key methods:
   - cv.wait(lock, predicate): 
     • Atomically releases lock and puts thread to sleep
     • Wakes up when notified AND predicate returns true
     • Automatically re-acquires lock before returning
     
   - cv.notify_all(): 
     • Wakes up ALL waiting threads
     • They check their predicate and re-sleep if still false

3. UNIQUE_LOCK (RAII lock wrapper)
   - Why not lock_guard? Because cv.wait() needs to unlock/relock
   - unique_lock allows temporary unlocking (lock_guard doesn't)

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
HOW IT WORKS - STEP BY STEP
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

ENQUEUE (when queue is full):
1. Thread acquires lock
2. cv.wait() checks: is queue NOT full?
   - NO → release lock, sleep, wait for notification
   - When notified → wake up, re-acquire lock, check again
   - YES → proceed
3. Add element to queue
4. cv.notify_all() → wake up threads waiting to dequeue

DEQUEUE (when queue is empty):
1. Thread acquires lock
2. cv.wait() checks: is queue NOT empty?
   - NO → release lock, sleep, wait for notification
   - When notified → wake up, re-acquire lock, check again
   - YES → proceed
3. Remove element from queue
4. cv.notify_all() → wake up threads waiting to enqueue

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
WHY NOTIFY_ALL (not notify_one)?
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
- notify_one() might wake the wrong type of waiter
- After enqueue, we want to wake DEQUEUERS, but might wake ENQUEUERS
- notify_all() is safer (slightly less efficient but correct)

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
INTERVIEW TALKING POINTS
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
- This is a classic Producer-Consumer pattern
- Real-world: Thread pools, message queues, work distribution
- Alternative: Use std::mutex + std::queue in production (C++20+)
*/

class BoundedBlockingQueue {
public:
    int capacity;
    queue<int>q;
    mutex mtx;                  // Protects queue from concurrent access
    condition_variable cv;      // Allows threads to wait/notify

    BoundedBlockingQueue(int capacity) {
        this->capacity = capacity;
    }
    
    void enqueue(int element) {
        // Step 1: Acquire exclusive lock on the mutex
        unique_lock<mutex>lock(mtx);
        
        // Step 2: Wait until queue is NOT full
        // cv.wait() will:
        //   - Check predicate (q.size() < capacity)
        //   - If FALSE: release lock, sleep, wait for notification
        //   - When notified: wake up, re-acquire lock, check again
        //   - If TRUE: proceed with lock held
        cv.wait(lock,[&](){return q.size()<capacity;});
        
        // Step 3: Queue is not full, safe to add element
        q.push(element);
        
        // Step 4: Notify waiting threads (dequeuers waiting for items)
        // notify_all() wakes ALL threads; they'll check their conditions
        cv.notify_all();
        
        // Lock automatically released when function exits (RAII)
    }
    
    int dequeue() {
        // Step 1: Acquire exclusive lock
        unique_lock<mutex>lock(mtx);
        
        // Step 2: Wait until queue is NOT empty
        // Same wait mechanism as enqueue
        cv.wait(lock,[&](){return q.size()>0;});
        
        // Step 3: Queue has items, safe to remove
        int top= q.front();
        q.pop();
        
        // Step 4: Notify waiting threads (enqueuers waiting for space)
        cv.notify_all();
        
        return top;
        // Lock automatically released here
    }
    
    int size() {
        // Even reading size needs lock to prevent race conditions
        // (e.g., reading size while another thread is modifying queue)
        unique_lock<mutex>lock(mtx);
        return q.size();
    }
};