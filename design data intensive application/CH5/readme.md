# Replication

Replication means keeping a copy of the same data on multiple machines that are connected via networks. Replication is used for multiple purposes:

* High availability
  Keeping the system running, even when one machine (or several) is down

* Disconncted operation
  Allowing an application to continue working when there is a network interruption

* Latency
  Placing data geographically close to users, so that users can interact with it faster

* Scalability
  Being able to handle a higher volume of reads than a single machine could handle, by performing reads on replicas

Three popular models: **single-leader**, **multi-leader**, and **leaderless**.

## Single Leader Model

### Leaders and Followers

Leaders are network nodes to handle write requests and send the new data to their followers for data replication. They are also called master or primary.

Followers are network nodes to subscribe leaders for replicating new data. They don't process write requests and serve as read-only node for external users.

### Synchronous and Asynchronous Replication

In synchronous replication, the leader responses the write request only after the replication at the follower node is finished. Key points:

* It ensures data is saved in at least two node (one leader and one follower).
* But it will have longer response time to wait for the response from the follower and less tolerance to system error and network failure.

In asynchronous replication, the leader immediately responses the write request regardless of the process of replication in its follower. Key points:

* It ensures fast response
* But it is unable to notify the user if the replication successes or not.

In practice, semi-synchronous replication is usually used to avoid the problems of both methods: have at least one replication done in the synchronous way, which ensures the data is saved at more than one place, then conduct asynchronous replication at other followers.

### Handling Node Outages

#### Follower

If a follower fails,a new follower can set up with a previous snapshot and pull latest data from the leader.

#### Leader

If a leader fails, one of the followers needs to be promoted as the new leader. The new leader could be chosen by election or assignment from previous leaders. Usually the follower with most up-to-date data is selected.

Note that sometimes the leader is not actually dead, but just slow because of network or peak work load. When the previous leader is back, the system must be configurated to make the previous become a follower.

#### Fundamental Problems

node failures, unreliable networks, trade-offs around replica consistency, durability, availability, latency

### Implementation of Replication Logs

#### Statement-based Replication

In this way, the leader will send the statement (like SQL) to insert data to its followers.

Though it is simple, it could create side effect if the statement is not fully self-contained (like using external packages or methods). Because it don't have absolute certainty, it is not a preferred method.

#### Write-ahead Log (WAL) Shipping

In this way, the log data will be sent from the leader to the follower in the format of storage engine.

Though it is straightforward, it uses a low level data structure from the storage engine (database) and makes engine migration pretty difficult.

#### Logical (Row-Based) Log Replication

In this way, the leader will send decoded data (likely per row) from its low level data structure to the followers. Since the decoded data is like the raw data, which doesn't depend on a specific storage engine, it is better for the system maintainability and operability.

### Problem with Replication Language

Asynchronous replication is a widely used method because of its high response speed. In this method, nodes in the system may have temporary data inconsistent, but will eventually synchronize data from the leader and reach the _eventual consistency_.

However, if the replication lag is large, special measures must be taken.

#### Reading Your Own Writes

The _read-after-write consistency_, or _read-your-writes consistency_, is a strategy to let the leader to response the read requests before the recent write is replicated. It ensure the current user does not read the old data from the replica.

#### Monotonic Reads

_Monotonic reads_ is a guarantee that a user will not read inconsistent data when doing random reads (from node with replication completed or not).

One way of doing this is to let a user always access to one node, not random nodes with different states.

#### Consistent Prefix Reads

_Consistent Prefix Reads_ is a guarantee that order data will be replicated with the same write order. This could be a problem because the network or system lag varies.

This can be done by add an order identifier, like timestamp, to the data.

## Multi-Leader Replication

In this model, there are more than one leaders in the network, which can accept writes and send notify followers to replicate.

It has some benefits over the single-leader model:

* Leaders can be located in different data center and closers to the users, thus performance is better.
* It has immediately available leaders if one of the leader fails.
* It has more tolerance to the network lag because a leader and its followers can be in a local network.
* It can serve offline operation, where a leader and its followers run without outside internet.

### Handling Write Conflicts

The biggest problem of multi-leader replication is write conflicts.

#### Synchronous & Asynchronous Conflict Detection

Though the conflict detection could be done in a synchronous way, for example, lock the write access before the current write finishes, this dismisses the advantage of multi-leader replication (write concurrently).

Therefore, asynchronous conflict detection is generally used: write first then detect conflict.

#### Conflict Avoidance

The simplest strategy, for exampling, limit the write access to reduce the possibility of conflict.

#### Converging to A consistent State

In the single-leader moder, writes are in order so that the final write must be the most current value.

But in the multi-leader model, there is no defined ordering of writes and the system needs to setup additional methods to help the system to determine what the final data is. It could be:

* provide an order identifier (like timestamp) to each write data
* save all write data and let the user decide which is the final value

#### Custom Conflict Resolution Logic

We can do the conflict resolution **on write**: set up the system to detect conflicts and resolve before writing values.

Or we can do it **on read**: set up the system to blindly save all writes and do the conflict resolution when the data is read.

## Leadless Model

In this model, every node could accept write request and notify data replication. Therefore, each node is both a follower and leader.

To have better replication, a node usually send replication requests to multiple nodes parallelly.

### Handling Node Failure

If a node fails, it may lose the latest data when it comes back. This creates data inconsistency in the network and it will give outdated data if the user requests data from it. A general practice is to send requests to multiple nodes to make sure at least on request will be send to a node with latest data.

#### Data Repair

**Read repair** is a strategy to check data consistency on write. If the client knows a replica has outdated data, send a write request with the latest data to that replica to fix the data inconsistency. This is suitable for the system with frequent read.

**Anti-entropy process** is a strategy to setup a background process to constantly detect data consistency in the background and resolve it.

#### Quorums for Reading and Writing

**Quorum** is a strategy to reduce the number of requests to the network and tolerate node failure. It specifies the required number of write responses and read responses to guarantee a level of node failure tolerance. For example, in a network with 5 node, if we requires 2 responses for a successful write and 4 responses for a successful read, then it can tolerate 2 node failure. Therefore we don't have to require all nodes to response and in general we only need w + r > n.

A further step can be taken to tolerate larger scale failure. If a large portion of the network fails and the system is unable to reach quorum for write or read, we can read/write the value in a node multiples time as if multiples copies of the value are stored in that node. This network compensation is called **Sloppy Quorums**.


# Notes
## Single-leader replication
- The first important choice that needs to be made is whether replication is synchronous or asynchronous.
- Almost all replication is done asynchronously, because synchronous replication introduces unbounded latency to the application.
- User-facing applications generally want to maintain the illusion of being synchronous, even when the underlying infrastructure is not.
- Asynchronous replication introduces a huge range of problems that you have to contend with if you try to maintain this illusion.
- The precise configuration of concerns with a single-leader replication strategy differs. At a minimum the leader handles writes, and communicates those writes to the follower, and then the followers provide reads.
- If a follower fails, you perform catch-up recovery. This is relatively easy (example, Redis).
- If a leader fails you have to perform a failover. This is very hard:
- If asynchronous replication is used the replica that is elected the leader may be "missing" some transaction history which occurred in the leader. Totally ordered consistency goes wrong.
- You can discard writes in this case, but this introduces additional complexity onto any coordinating services that are also aware of those writes (such as cache invalidation).
- It is possible for multiple nodes to believe they are the leader (Byzantine generals). This is dangerous as it allows concurrent writes to different parts of the system. E.g. split brain.
- Care must be taken in setting the timeout. Timeouts that are too long will result in application delay for users. Timeouts that are too short will cause unnecessary failovers during high load periods.
- Every solution to these problems requires making trade-offs!

## Replication streams
- How do you implement the replication streams?
- The immediately obvious solution is statement-based replication. Every write request results in a follower update.
- This is a conceptually simple architecture and likely the most compact and human-readable one. But it's one that's fallen out of favor due to problems with edge cases.
- Statement-based replication requires pure functions. Non-deterministic functions (RAND, NOW), auto-incrementation (UPDATE WHERE), and side effects (triggers and so on) create difficulties.
- Write-ahead log shipping is an alternative where you ship your write-ahead log (if you're a database or something else with a WAL).
- This is nice because it doesn't require any additional work by the service. A WAL already exists.
- This deals with the statement-based replication problems because WALs contain record dumps by design. To update a follower, push that record dump to it.
- The tradeoff is that record dumps are a data storage implementation detail. Data storage may change at any times, and so the logical contents of the WAL may differ between service versions.
- The write-ahead logs of different versions of a service are generally incompatible! But your system will contain many different versions.
- Generally WALs are not designed to be version-portable, so upgrading distributed services using WAL shipping for replication requires application downtime.
- Logical log replication is the use of an alternative log format for replication. E.g. replication is handled by its own distinct logs that are only used for that one specific purpose.
- This is more system that you have to maintain, but it's naturally backwards and forward compatible if you design it right (using e.g. a data interchange format like Protobufs) and works well in practice.
- Post-script: you may want partial replication. In that case you generally need to have application-level replication. You can do this in databases, for example, by using triggers and stored procedures to move data that fits your criteria in specific ways.
- Asynchronously replicated systems are naturally eventually consistent. No guarantees are made for when "eventually" will come to pass.
- Replication lag occurs in many different forms. Some specific types of lag can be mitigated (at the cost of performance and complexity), if doing so is desirable for the application (but you can only recover a fully synchronous application by being fully synchronous).
- The book cites three examples of replication lag that are the three biggest concerns for systems.
The first one: your user expects that data they write they can immediately read. But if they write to a leader and read from a replica, and the replica falls behind the leader, they may not see that data.
- A system that guarantees users can see their own modified data on refresh is one which provides read-your-write consistency. In practice, most applications want to provide this!
- You can implement read-your-writes in a bunch of different ways, but the easiest way is to simply have the leader handle read requests for user-owned data.
- Another issue occurs if a user is reading from replica, and then switches to another replica that is further out of sync than the previous one. This creates the phenomenon of going backwards in time, which is bad.
- Applications which guarantee this doesn't happen provide monotonic reads.
- An easy way to get monotonic reads is to have the user always read from the same replica (e.g. instead of giving them a random one).
- The third kind of inconsistency is causal inconsistency. Data may get written to one replica in one order, and to another in a different order. Users that move replicas will see their data rearranged.
- Globally ordered reads require that the system be synchronous, so we can't recover all of the order. However, what we can do is gaurantee that events that have a causal relationship (e.g. this event happened, which caused this event to happen) are mirrored correctly across all nodes.
- This is a weaker gaurantee than fully synchronous behavior because it only covers specific subsequences (user stories) from the data, not the entire sequence as a whole.
- A system that handles this problem is said to provide consistent prefix reads.
- It's notable that you can avoid all of these problems if you implement transactions. Transactions are a classical database feature that was oft-dropped in the new world because it is too slow and non-scalable for a distributed system to implement. They seem to be making a comeback now, but it's still a wait-and-see.
