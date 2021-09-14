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
