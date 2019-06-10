P4
- Many new tools for data storage and processing have emerged in recent years.
- Secondly, increasingly many applications now have such demanding or wide-ranging
requirements that a single tool can no longer meet all of its data processing and storage
needs.


P6
- Reliability
The system should continue to work correctly (performing the correct function at
the desired level of performance) even in the face of adversity (hardware or software
faults, and even human error). See “Reliability” on page 6.

- Scalability
As the system grows (in data volume, traffic volume, or complexity), there should
be reasonable ways of dealing with that growth.

Twitter example: P12
Approach 1: Posting a tweet simply inserts the new tweet into a global collection of tweets. When a user requests their home timeline, look up all the people they follow, find all the tweets for each of those users, and merge them (sorted by time)

Approach 2: Maintain a cache for each user’s home timeline—like a mailbox of tweets for
each recipient user. When a user posts a tweet, look up all the
people who follow that user, and insert the new tweet into each of their home
timeline caches

The first version of Twitter used approach 1, but the systems struggled to keep up
with the load of home timeline queries, so the company switched to approach 2. This
works better because the average rate of published tweets is almost two orders of
magnitude lower than the rate of home timeline reads, and so in this case it’s preferable
to do more work at write time and less at read time.

The final twist of the Twitter anecdote: now that approach 2 is robustly implemented,
Twitter is moving to a hybrid of both approaches. Most users’ tweets continue to be
fanned out to home timelines at the time when they are posted, but a small number
of users with a very large number of followers (i.e., celebrities) are excepted from this
fan-out. Tweets from any celebrities that a user may follow are fetched separately and
merged with that user’s home timeline when it is read, like in approach 1. This hybrid
approach is able to deliver consistently good performance. We will revisit this example
in Chapter 12 after we have covered some more technical ground.


-- Describe performance (P13)

percentiles are often used in service level objectives (SLOs) and service
level agreements (SLAs), contracts that define the expected performance and availability
of a service.

-- Approaches for Coping with Load(P17)
Some systems are elastic, meaning that they can automatically add computing resources
when they detect a load increase

- Maintainability
Over time, many different people will work on the system (engineering and operations,
both maintaining current behavior and adapting the system to new use
cases), and they should all be able to work on it productively.

Operability: Making Life Easy for Operations (P19)
