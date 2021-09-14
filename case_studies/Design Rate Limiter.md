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
