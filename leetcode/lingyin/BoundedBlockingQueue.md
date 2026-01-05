# Bounded Blocking Queue Implementation

This is a **thread-safe bounded blocking queue** implementation in C++. A bounded blocking queue is a thread-safe data structure with a maximum capacity. When the queue is full, threads trying to enqueue will block (wait). When empty, threads trying to dequeue will block.

## Implementation

```cpp
class BoundedBlockingQueue {
public:
    int capacity;
    queue<int>q;
    mutex mtx;
    condition_variable cv;

    BoundedBlockingQueue(int capacity) {
        this->capacity = capacity;
    }
    
    void enqueue(int element) {
        unique_lock<mutex>lock(mtx);
        cv.wait(lock,[&](){return q.size()<capacity;});
        q.push(element);
        cv.notify_all();
    }
    
    int dequeue() {
        unique_lock<mutex>lock(mtx);
        cv.wait(lock,[&](){return q.size()>0;});
        int top= q.front();
        q.pop();
        cv.notify_all();
        return top;
    }
    
    int size() {
        return q.size();
    }
};
```

## Member Variables

### `int capacity`
- Stores the maximum number of elements the queue can hold
- Acts as the upper bound for the queue size

### `queue<int> q`
- The underlying container storing the actual elements
- Standard C++ queue (FIFO - First In First Out)

### `mutex mtx`
- A mutual exclusion lock that ensures only one thread can access the queue at a time
- Prevents race conditions when multiple threads try to modify the queue simultaneously

### `condition_variable cv`
- Allows threads to wait efficiently for certain conditions to become true
- Enables threads to sleep instead of busy-waiting (spinning)
- Used to signal when the queue state changes (not full / not empty)

## Constructor

```cpp
BoundedBlockingQueue(int capacity) {
    this->capacity = capacity;
}
```
- Initializes the queue with a specified maximum capacity
- Other members (queue, mutex, condition_variable) are default-constructed

## `enqueue(int element)` Method

```cpp
void enqueue(int element) {
    unique_lock<mutex>lock(mtx);
    cv.wait(lock,[&](){return q.size()<capacity;});
    q.push(element);
    cv.notify_all();
}
```

**Step-by-step execution:**

1. **`unique_lock<mutex>lock(mtx)`**
   - Acquires the mutex lock, blocking other threads from entering enqueue/dequeue
   - `unique_lock` is RAII-based - automatically releases when it goes out of scope

2. **`cv.wait(lock,[&](){return q.size()<capacity;})`**
   - **Wait condition**: `q.size() < capacity` (queue is not full)
   - If the queue IS full (`q.size() == capacity`):
     - Atomically releases the lock and puts the thread to sleep
     - Thread waits until another thread calls `notify_all()` on the same condition variable
     - When woken up, re-acquires the lock and rechecks the condition
     - If still full, goes back to sleep (spurious wakeup protection)
   - If the queue is NOT full:
     - Proceeds immediately without blocking

3. **`q.push(element)`**
   - Adds the element to the queue
   - Only executes when there's space available

4. **`cv.notify_all()`**
   - Wakes up ALL threads waiting on this condition variable
   - Alerts threads blocked in `dequeue()` that the queue is no longer empty
   - Also alerts other `enqueue()` threads (though they'll just check and go back to sleep if still full)

## `dequeue()` Method

```cpp
int dequeue() {
    unique_lock<mutex>lock(mtx);
    cv.wait(lock,[&](){return q.size()>0;});
    int top= q.front();
    q.pop();
    cv.notify_all();
    return top;
}
```

**Step-by-step execution:**

1. **`unique_lock<mutex>lock(mtx)`**
   - Acquires the mutex lock

2. **`cv.wait(lock,[&](){return q.size()>0;})`**
   - **Wait condition**: `q.size() > 0` (queue is not empty)
   - If the queue IS empty:
     - Releases lock and sleeps until notified
     - Re-checks condition when woken up
   - If the queue is NOT empty:
     - Proceeds immediately

3. **`int top = q.front()`**
   - Gets the front element (doesn't remove it yet)

4. **`q.pop()`**
   - Removes the front element from the queue

5. **`cv.notify_all()`**
   - Wakes up ALL waiting threads
   - Alerts threads blocked in `enqueue()` that space is now available
   - Also alerts other `dequeue()` threads

6. **`return top`**
   - Returns the dequeued value

## `size()` Method

```cpp
int size() {
    return q.size();
}
```

⚠️ **This method has a thread-safety issue!**
- It reads `q.size()` without acquiring the mutex
- Another thread could be modifying the queue simultaneously
- Could return inconsistent/stale values or cause undefined behavior
- **Should be**: 
  ```cpp
  int size() {
      unique_lock<mutex>lock(mtx);
      return q.size();
  }
  ```

## Key Concepts

### Why `notify_all()` instead of `notify_one()`?
- With a single condition variable for both "not full" and "not empty" conditions, using `notify_one()` could wake the wrong type of waiter
- `notify_all()` ensures both enqueue and dequeue waiters get notified
- Each thread rechecks its own condition and goes back to sleep if not satisfied

### Lambda in `cv.wait()`
- `[&](){return q.size()<capacity;}`
- The `[&]` captures all local variables by reference
- The condition is checked in a loop to handle spurious wakeups
- Prevents the "lost wakeup" problem

### RAII Pattern
- `unique_lock` automatically releases the mutex when it goes out of scope
- Exception-safe: mutex released even if an exception is thrown

## Use Cases

This implementation provides **thread-safe**, **blocking** producer-consumer semantics with automatic thread coordination:

- **Producer threads** call `enqueue()` and block if queue is full
- **Consumer threads** call `dequeue()` and block if queue is empty
- Perfect for multi-threaded scenarios where you need bounded buffering between producers and consumers

## Time Complexity

- **enqueue()**: O(1) average case
- **dequeue()**: O(1) average case
- **size()**: O(1)

## Space Complexity

O(capacity) - bounded by the maximum capacity specified at construction.
