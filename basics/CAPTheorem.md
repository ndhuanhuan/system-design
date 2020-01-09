## CAP Theorem

CAP theorem states that it is impossible for a distributed software system to simultaneously provide more than two out of three of the following guarantees (CAP): Consistency, Availability and Partition tolerance. When we design a distributed system, trading off among CAP is almost the first thing we want to consider. CAP theorem says while designing a distributed system we can pick only two of:

* **Consistency:** All nodes see the same data at the same time. Consistency is achieved by updating several nodes before allowing further reads.
* **Availability:** Every request gets a response on success/failure. i.e At no point in time the system shall ask the user to come back later. Availability is achieved by replicating the data across different servers.
* **Partition Tolerance:** System continues to function even if the communication fails between nodes. This is a property of a distributed system to recover from node failures. i.e. Some nodes being cut off from the system (Ex: A network cable cut off, power outage in a data center) and when they come back online it should work seamlessly.


![cap](https://user-images.githubusercontent.com/6800366/37402406-710e8dbc-27b1-11e8-987c-bb07cf2e3e94.png)

Make a decision to choose between consistency or availability.

* **CP System:** When a request comes in and if the recent record is not available then don’t give the update yet. Ex: Amazon's Dynamo DB’s Strong Consistency provides the most recent record with some latency i.e. eventual consistency which is faster but provides an older record.

* **AP System:** When a request is made, give an update immediately. May be the record was a minute older but availability is more important than consistency. Ex: Uber driver location feature. Incase no data is transmitted from driver’s GPS device to the Uber servers, Uber choose to give the users old location data of the driver instead of not giving any update at all.

For a non-distributed system like a RDBMS, CA option can be applicable where partition tolerance is not applicable.
