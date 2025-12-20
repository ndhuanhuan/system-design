# Reliable, Scalable, and Maintainable Applications

## Reliability

The ability of a system to be tolerate faults:
* Hardware faults
* Software faults
* Human errors

## Scalability

The ability of a system to handle increased load.

*Load parameter* is a term to describe system load, which depends on the purpose of a system. For example, requests per second, simultaneous active connections, hits for cache, etc.

Performance can be viewed in two ways:
* increase load parameter and keep the system resource, see how the system is affected
* increase load parameter, see ow much resource is need to keep system performance unchanged

*Throughput*, the number of records a system can process per second, is a metrics for batch(?) processing system, like hadoop.

Percentile is usually used to measure request response time. High percentile is also known as *tail latencies*, which are worst response time.

*Service level object (SLO)* and *service level argreement (SLA)* are used to define expected performance. Percentile is usually to define such goals, like 99& of uses should have X response time.

Two ways to cope with load:
* scale-up (more powerful machine)
* scale-out (more machines)

## Maintainability

The ability to make a system simpler to maintain.
* Operability: make life easier for operators
* Simplicity: manage complexity
* Enolvability: make change easy

## Example

### Twitter (p11)

Two different system designs for different users:
* those mostly being subscribed
* those mostly subscribing

A good example on how to balance write/load message to broadcast

###  Amazon (p15)

Special focus on tail latencies as users with slower response usually have more data in the application, which means they are high value customer.
## Summary

This chapter introduces the foundational concepts for building reliable, scalable, and maintainable applications:

- **Reliability:** Systems must tolerate hardware, software, and human faults to ensure continuous operation.
- **Scalability:** Systems should handle increased load by scaling up (stronger machines) or scaling out (more machines), with performance measured by throughput and response time percentiles.
- **Maintainability:** Systems should be easy to operate, simple to understand, and adaptable to change.

Real-world examples like Twitter and Amazon illustrate how these principles are applied to address different user needs and performance challenges, especially focusing on tail latencies and system design trade-offs.
