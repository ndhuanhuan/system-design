# Interview Prep - Concept Questions

## Operating Systems

### Process vs Thread
**Process**: Independent program with its own memory space (heap, stack, code, data). Heavy-weight, isolated from other processes. Context switching is expensive.

**Thread**: Lightweight execution unit within a process. Shares memory space (heap, code, data) with other threads but has its own stack and registers. Context switching is cheaper. Enables concurrent execution within same process.

### Virtual Memory
Abstraction that gives each process its own private address space (e.g., 0 to 2^64 on 64-bit systems), isolating processes from each other. The OS and MMU (Memory Management Unit) translate virtual addresses to physical RAM addresses or disk storage (swap space).

**How It Works**:
Memory is divided into fixed-size **pages** (typically 4KB). Each process has a **page table** that maps virtual page numbers to physical page frames in RAM. When a process accesses memory:
1. CPU generates virtual address (e.g., `0x7fff8000`)
2. MMU looks up page table to find physical address
3. If page in RAM → access data (page hit)
4. If page not in RAM → page fault → OS loads from disk (page miss)

**Benefits**: 
- **Process isolation/security**: Process A at virtual address `0x1000` and Process B at `0x1000` map to different physical addresses. Prevents processes from corrupting each other's memory.
- **Allows processes larger than physical RAM**: If you have 8GB RAM but run a 12GB process, OS swaps unused pages to disk.
- **Efficient memory usage via paging**: Multiple processes share physical RAM. Unused pages aren't loaded until needed (demand paging).
- **Simplifies memory management**: Every process sees continuous address space starting at 0, regardless of physical RAM fragmentation.

**Example Scenario**:
- Physical RAM: 4GB (addresses 0x0 to 0xFFFFFFFF)
- Process A uses virtual address `0x0000 1000`, mapped to physical `0x1234 5000`
- Process B uses virtual address `0x0000 1000`, mapped to physical `0x9ABC D000`
- Both processes think they own `0x1000`, but physically they're in different RAM locations

**Page Table**: Multi-level data structure (e.g., 4-level on x86-64) where each entry contains:
- Physical frame number
- Present bit (in RAM or on disk?)
- Dirty bit (modified?)
- Access permissions (read/write/execute)

The MMU hardware does this translation automatically on every memory access, with a TLB (Translation Lookaside Buffer) cache for speed.

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
- **Flow control**: Prevents sender from overwhelming receiver. Receiver advertises a "receive window" size indicating available buffer space. If receiver's buffer is full, sender pauses until space is available. Uses sliding window protocol.
  - *Example*: Fast server sending to slow client. Client's buffer fills up → client sets window size to 0 → server stops sending until client processes data.
- **Congestion control**: Prevents overwhelming the network (routers/switches between sender and receiver). TCP detects packet loss as congestion signal and reduces sending rate. Algorithms: Slow Start, Congestion Avoidance, Fast Retransmit.
  - *Example*: Network is congested → packets dropped → sender detects timeout/duplicate ACKs → reduces sending rate → network recovers.
- **Error checking**: Each packet has checksum to detect corruption. Receiver verifies checksum. If corrupted or missing, receiver doesn't ACK → sender retransmits after timeout.
  - *Example*: Packet corrupted during transmission → checksum mismatch → dropped → sender retransmits.
- Higher overhead, slower (3-way handshake, ACKs, retransmissions)
- Use case: HTTP, email, file transfer (data integrity matters)

**UDP** (User Datagram Protocol):
- Connectionless, unreliable (no delivery guarantee, no ordering)
- No flow/congestion control, minimal overhead (just send packets)
- No error recovery (has checksum but drops bad packets, doesn't retransmit)
- Faster, lower latency
- Use case: Video streaming (tolerate loss), gaming (real-time), DNS (small requests)

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