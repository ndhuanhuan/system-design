---
title: Design patterns for microservices
created: '2019-12-29T20:46:56.577Z'
modified: '2019-12-30T01:39:15.408Z'
---

# Design patterns for microservices
## Design Patterns
- Service discovery
- Edge server
- Reactive microservices
- Central configuration
- Centralized log analysis
- Distributed tracing
- Circuit Breaker
- Control loop
- Centralized monitoring and alarms

## Service Discovery
- Problem: How can clients find microservices and their instances?How can clients find microservices and their instances?
- Solution: 
1. Client-side routing: The client uses a library that communicates with the service discovery service to find out the proper instances to send the requests to.
2. Server-side routing: The infrastructure of the service discovery service also exposes a reverse proxy that all requests are sent to. The reverse proxy forwards the requests to a proper microservice instance on behalf of the client.

## Edge Server
- Problem: In a system landscape of microservices, it is in many cases desirable to expose some of the microservices to the outside of the system landscape and hide the remaining microservices from external access. The exposed microservices must be protected against requests from malicious clients.
- Solution: Add a new component, an Edge Server, to the system landscape that all incoming requests will go through: Implementation notes: An edge server typically behaves like a reverse proxy and can be integrated with a discovery service to provide dynamic load balancing capabilities. 

## Reactive Microservice
- Problem: Traditionally, as Java developers, we are used to implementing synchronous communication using blocking I/O, for example, a RESTful JSON API over HTTP. Using a blocking I/O means that a thread is allocated from the operating system for the length of the request. If the number of concurrent requests goes up (and/or the number of involved components in a request, for example, a chain of cooperating microservices, goes up), a server might run out of available threads in the operating system, causing problems ranging from longer response times to crashing servers.

- Solution: Use non-blocking I/O to ensure that no threads are allocated while waiting for processing to occur in another service, that is, a database or another microservice.

## Central configuration
- Problem: How do I get a complete picture of the configuration that is in place for all the running microservice instances? How do I update the configuration and make sure that all the affected microservice instances are updated correctly?

- Solution: Add a new component, a configuration server, to the system landscape to store the configuration of all the microservices.

## Centralized log analysis
- Problem: Centralized log analysis has the following problem, solution, and solution requirements.

- Solution: Add a new component that can manage centralized logging 

## Distributed Tracing
- Problem: It must be possible to track requests and messages that flow between microservices while processing an external call to the system landscape.

- Solution: To track the processing between cooperating microservices, we need to ensure that all related requests and messages are marked with a common correlation ID and that the correlation ID is part of all log events. Based on a correlation ID, we can use the centralized logging service to find all related log events.

## Circuit Breaker
- Problem: A system landscape of microservices that uses synchronous intercommunication can be exposed to a chain of failure. If one microservice stops responding, its clients might get into problems as well and stop responding to requests from their clients. The problem can propagate recursively throughout a system landscape and take out major parts of it.

- Solution: Add a Circuit Breaker that prevents new outgoing requests from a caller if it detects a problem with the service it calls. 

## Control Loop
- Problem: In a system landscape with a large number of microservice instances spread out over a number of servers, it is very difficult to manually detect and correct problems such as crashed or hung microservice instances.

- Solution: Add a new component, a control loop, to the system landscape; this constantly observes the actual state of the system landscape; compares it with the desired state, as specified by the operators; and, if required, takes action. For example, if the two states differ, it needs to make the actual state equal to the desired state.

## Centralized monitoring and alarms

- Problem: If observed response times and/or the usage of hardware resources become unacceptably high, it can be very hard to discover the root cause of the problem. For example, we need to be able to analyze hardware resource consumption per microservice.

- Solution: To curb this, we add a new component, a monitor service, to the system landscape, which is capable of collecting metrics about hardware resource usage for each microservice instance level.

