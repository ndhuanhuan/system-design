# Definetion
A rate
limiter, at a high-level, limits the number of events an entity (user, device, IP,
etc.) can perform in a particular time window. For example:
- A user can send only one message per second.
- A user is allowed only three failed credit card transactions per day.
- A single IP can only create twenty accounts per day.
In general, a rate limiter caps how many requests a sender can issue in a specific
time window. It then blocks requests once the cap is reached.

# Why do we need API rate limiting?
- Misbehaving clients/scripts: Either intentionally or unintentionally,
some entities can overwhelm a service by sending a large number of
requests. Another scenario could be when a user is sending a lot of
lower-priority requests, and we want to make sure that it doesn’t affect the
high-priority traffic. For example, users sending a high volume of requests
for analytics data should not be allowed to hamper critical transactions for
other users.
- Security: By limiting the number of the second-factor attempts (in 2-factor
auth) that the users are allowed to perform, for example, the number of
times they’re allowed to try with a wrong password.
- To prevent abusive behavior and bad design practices: Without API
limits, developers of client applications would use sloppy development
tactics, for example requesting the same information over and over again.
- To keep costs and resource usage under control: Services are generally
designed for normal input behavior, for example, a user writing a single
post in a minute. Computers could easily push thousands/second through
an API. Rate limiter enables controls on service APIs.
- Revenue: Certain services might want to limit operations based on the tier
of their customer’s service, and thus create a revenue model based on rate
limiting. There could be default limits for all the APIs a service offers. To go
beyond that, the user has to buy higher limits
- To eliminate spikiness in traffic: So that a service stays up for everyone
else.

# Requirements and Goals of the System
## Functional Requirements:
1. Limit the number of requests an entity can send to an API within a time
window, e.g., 15 requests per second.
2. The APIs are accessible through a cluster, so the rate limit should be
considered across different servers. The user should get an error message
whenever the defined threshold is crossed within a single server or across a
combination of servers.
## Non-Functional Requirements:
1. The system should be highly available. The rate limiter should always work
since it protects our service from external attacks.
2. Our rate limiter should not introduce substantial latencies affecting the
user experience.

# How to do Rate Limiting?
Rate Limiting is a process that is used to define the rate and speed at which
consumers can access APIs. Throttling is the process of controlling the usage of
the APIs by customers during a given period. Throttling can be defined at the
application level and/or API level. When a throttle limit is crossed, the server
returns “429” as HTTP status to the user with message content as “Too many
requests”.

# What are different types of throttling
- Hard Throttling: The number of API requests cannot exceed the throttle limit.
- Soft Throttling: In this type, we can set the API request limit to exceed a certain
percentage. For example, if we have rate-limit of 100 messages a minute and 10%
exceed limit. Our rate limiter will allow up to 110 messages per minute.
- Elastic or Dynamic Throttling : Under Elastic throttling, the number of requests
can go beyond the threshold if the system has some resources available. For
example, if a user is allowed only 100 messages a minute, we can let the user send
more than 100 messages a minute if there are free resources available in the
system.

# What are different types of algorithms used for Rate Limiting?
Following are the two types of algorithms used for Rate Limiting:

**Fixed Window Algorithm:** In this algorithm, the time window is considered from the start of the time-unit to 
the end of the time-unit. For example, a period would be considered 0-60 seconds for a minute irrespective of 
the time frame at which the API request has been made. In the diagram below, there are two messages 
between 0-1 second and three messages between 1-2 seconds. If we have a rate limiting of two messages a 
second, this algorithm will throttle only ‘m5’.

![rate-limiting-algorithms-fixed-window-and-rolling-window](https://github.com/sm2774us/System_Design/raw/0a6e1afd89ed07f4a4566dc6da48afb39ccfe225/009_Designing_an_API_Rate_Limiter/assets/rate-limiting-algorithms-fixed-window-and-rolling-window.PNG)

**Rolling Window Algorithm:** In this algorithm, the time window is considered from the fraction of the time at 
which the request is made plus the time window length. For example, if there are two messages sent at the 
300th millisecond and 400th millisecond of a second, we’ll count them as two messages from the 
300th millisecond of that second up to the 300th millisecond of next second. In the above diagram, keeping 
two messages a second, we’ll throttle ‘m3’ and ‘m4’.
