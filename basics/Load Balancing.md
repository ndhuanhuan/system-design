---
title: Load Balancing
created: '2019-12-30T21:56:12.202Z'
modified: '2019-12-31T00:33:48.192Z'
---

# Load Balancing
## Basics
It helps to  distribute load across multiple resources according to some metric (random, round-robin, random with weighting for memory or CPU utilization, etc.). LB also keeps track of the status of all the resources while distributing requests. If a
server is not available to take new requests or is not responding or has elevated error rate, LB will stop sending traffic to such a server.

We can add LBs at three places:
- Between the user and the web server
- Between web servers and an internal platform layer, like application servers or cache servers
- Between internal platform layer and database.

## Implementations
### Smart Clients

A smart client will take a pool of service hosts and balances load across them. It
also detects hosts that are not responding to avoid sending requests their way.
Smart clients also have to detect recovered hosts, deal with adding new hosts, etc.
Adding load-balancing functionality into the database (cache, service, etc.) client
is usually an attractive solution for the developer. It looks easy to implement and
manage especially when the system is not large. But as the system grows, LBs
need to be evolved into standalone servers.

### Hardware LB
- Expensive
- very high performance
- not trivial to configure
- only used as the first point of contact for user requests to their infrastructure and use other mechanismsonly

### Software Load Balancers
#### Software Load Balancers in Client
This is probably the cheapest way to implement load balancing . In this case, all logic for your load balancing resides on the client application. On startup, the client application (Eg. A mobile phone app) is provided with a list of web servers / application servers it can communicate with. The client app picks the first one in the list and requests data from the server. If a failure is detected persistently (after a configurable number of retries), it marks the first server as unavailable and picks another server from the list to request data from.

#### Software Load Balancers in Services
Software load balancers are pieces of software that receives a set of requests and routes them according to a set of rules. Unlike hardware load balancers, software load balancers do not require any specific

Type of hardware – they can be installed on any windows or linux machines. One has the option of either using an off-the-shelf software load balancer like HA Proxy or writing their own custom software for

Load balancing specific workload types. For example, when designing the Authentication platform for Microsoft Office365, we wrote a custom load balancer to load balance Active Directory Queries.


## How to use load balancing during system design interviews?

- Load balancing enables elastic scalability and redundancy (you can have many copies of the same data). Elastic scalability improves performance and throughput of data. Redundancy improves availability and also helps in backup/restore of service in case a few servers fail.
- Load balancers can be placed at any software layer
- Load balancers can be implemented in hardware or software. Many companies uses both at different scale points in their system.

## Common Algorithms
- Round Robin
- Weighted Round Robin
- Least Connection
- Fastest Response
- IP Hash
- URL hash
- Consistent Hashing(* important)

## Good Reading Resources
- https://www.acodersjourney.com/system-design-interview-load-balancing/
- https://tianpan.co/notes/182-designing-l7-load-balancer
