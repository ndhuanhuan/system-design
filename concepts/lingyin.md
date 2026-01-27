# Interview Prep - Concept Questions

## Operating Systems

### Process vs Thread
**Process**: Independent program with its own memory space (heap, stack, code, data). Heavy-weight, isolated from other processes. Context switching is expensive.

**Thread**: Lightweight execution unit within a process. Shares memory space (heap, code, data) with other threads but has its own stack and registers. Context switching is cheaper. Enables concurrent execution within same process.

### Virtual Memory
Abstraction that gives each process its own private address space, isolating processes from each other. Maps virtual addresses to physical RAM or disk (swap).

**Benefits**: 
- Process isolation/security
- Allows processes larger than physical RAM
- Efficient memory usage via paging
- Simplifies memory management

**Page Table**: Maps virtual pages to physical frames. OS/MMU handles translation via page table lookups.

### Exception (异常)
Unexpected event during program execution that disrupts normal flow. Examples: divide by zero, null pointer dereference, out of memory. Can be caught and handled (try-catch) or propagate up the call stack. Different from errors (compile-time) and interrupts (hardware signals).

## Data Structures

### Array vs Linked List (数组 vs 链表)
**Array**:
- Contiguous memory, O(1) random access
- Fixed size (or expensive resize), cache-friendly
- Poor insertion/deletion at beginning/middle O(n)

**Linked List**:
- Non-contiguous, O(n) access by index
- Dynamic size, O(1) insertion/deletion at known position
- Extra memory for pointers, cache-unfriendly

## Networking

### TCP vs UDP
**TCP** (Transmission Control Protocol):
- Connection-oriented, reliable (guaranteed delivery, ordering)
- Flow control, congestion control, error checking
- Higher overhead, slower
- Use case: HTTP, email, file transfer

**UDP** (User Datagram Protocol):
- Connectionless, unreliable (no delivery guarantee)
- No flow/congestion control, minimal overhead
- Faster, lower latency
- Use case: Video streaming, gaming, DNS

## Databases

### Transaction (事务)
Set of operations that execute as a single atomic unit. Either all succeed (commit) or all fail (rollback).

**ACID Properties**:
- **Atomicity**: All or nothing
- **Consistency**: Database stays in valid state
- **Isolation**: Concurrent transactions don't interfere
- **Durability**: Committed changes persist even after crashes

### Write-Through vs Write-Back Cache
**Write-Through**:
- Write to cache AND database simultaneously
- Slower writes, but data consistency guaranteed
- Lower risk of data loss

**Write-Back** (Write-Behind):
- Write to cache only, async write to database later
- Faster writes, better performance
- Risk of data loss if cache crashes before flush

### Which Database to Use
**SQL (Relational)**:
- Structured data, complex joins, ACID transactions
- Examples: PostgreSQL, MySQL
- Use: Banking, inventory systems

**NoSQL**:
- **Document**: Flexible schema, nested data (MongoDB) - CMS, catalogs
- **Key-Value**: Simple, fast lookups (Redis, DynamoDB) - Session store, caching
- **Column**: Wide tables, analytics (Cassandra, HBase) - Time series, logs
- **Graph**: Relationships (Neo4j) - Social networks, recommendations

## System Design

### CAP Theorem
Distributed system can guarantee only 2 of 3:
- **Consistency**: All nodes see same data at same time
- **Availability**: Every request gets response (success/failure)
- **Partition Tolerance**: System works despite network failures

**Trade-offs**:
- CP: Consistent but may reject requests during partition (HBase, MongoDB)
- AP: Available but may serve stale data (Cassandra, DynamoDB)
- CA: Not realistic in distributed systems (need P)

### Design Patterns & Importance
Reusable solutions to common software design problems.

**Examples**:
- **Singleton**: One instance (logger, config)
- **Factory**: Object creation abstraction
- **Observer**: Event-driven communication
- **Strategy**: Interchangeable algorithms

**Why Important**: Code reusability, maintainability, scalability, common vocabulary for teams.

### API with 10K QPS Write Traffic
**Key considerations**:
1. **Load Balancing**: Distribute across multiple servers (nginx, ALB)
2. **Database**: Sharding/partitioning for horizontal scaling
3. **Async Processing**: Queue (Kafka, RabbitMQ) + workers for non-critical writes
4. **Caching**: Redis for read-heavy patterns, reduce DB load
5. **Rate Limiting**: Protect from abuse (token bucket, sliding window)
6. **Monitoring**: Track latency, error rates (Prometheus, DataDog)
7. **Auto-scaling**: Add capacity during spikes

## Concurrency & Parallelism

### Concurrency vs Parallelism
**Concurrency**: Multiple tasks making progress in overlapping time periods. Single core can context-switch between tasks. About dealing with multiple things at once (structure).

**Parallelism**: Multiple tasks executing simultaneously on multiple cores. Requires multiple processors. About doing multiple things at once (execution).

**Example**: Concurrency = one person juggling multiple tasks. Parallelism = multiple people each doing one task.

### Multithreading Synchronization
**Problem**: Race conditions when multiple threads access shared data.

**Solutions**:
- **Mutex/Lock**: Exclusive access to critical section
- **Semaphore**: Control access to limited resources (counting)
- **Condition Variables**: Thread waits until condition is met
- **Atomic Operations**: Lock-free operations (compare-and-swap)
- **Read-Write Locks**: Multiple readers or single writer

**Best Practices**: Minimize lock scope, avoid deadlock (lock ordering), prefer higher-level constructs (concurrent data structures).