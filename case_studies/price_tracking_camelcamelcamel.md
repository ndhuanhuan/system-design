# Price tracking service like CamelCamelCamel

CamelCamelCamel is a price tracking service that monitors Amazon product prices over time and alerts users when prices drop below their specified thresholds. It also has a popular Chrome extension with 1M active users that displays price history directly on Amazon product pages, allowing for one-click subscription to price drop notifications without needing to leave the Amazon product page.

## Questions

- For this system, users should be able to: view price history for Amazon products (via website or Chrome extension) and subscribe to price drop notifications with thresholds (via website or Chrome extension)?
  - Yes, all of this
 
- How many products should the system manage?
  - 500M Amazon products

- What is the latency requirement for users requesting for price drops or price history?
  - The system should respond price drops or price history within 500 ms
 
- Within how long does the system need to deliver price drop notifications when prices change?
  - The system needs to deliver price drop notifications within 1 hr of price change
 
## Functional

- Users should be able to view price history for Amazon products (via website or Chrome extension)
- Users should be able to subscribe to price drop notifications with thresholds (via website or Chrome extension)

Out of scope:

- Search and discover products on the platform
- Price comparison across multiple retailers

## Non-functional

- Availability > consistency:
  - The system can still tolerate eventual consistency of price changes (it allows 1 hr of notification delivery), however it needs to manage 500M of products, where availability and scalability are important
- Low latency of price drop or history requests within 500 ms
- Price drop notifications delivered within 1 hr from change

Note: The scale and performance requirements for CamelCamelCamel are driven by Amazon's massive product catalog and the need for timely price notifications

Out of scope:

- Real-time price updates

<br/>

Note: We're building a system that must be "polite" to Amazon while providing valuable price tracking to millions of users. This creates interesting technical challenges around data collection, storage efficiency, and notification delivery that we'll address in our deep dives.

## Data model / entities

- Products:
  - This entity represents an Amazon product we're tracking, including its current price and metadata needed for price history display and notifications
    - productID
    - name
    - currentPrice
    - updatedTime

- Subscription:
  - Subscriptions links users to productIDs they want to monitor, specifying the price threshold that triggers notifications
    - subID
    - userID
    - productID
    - threshold

- Price history:
  - This time-series entity captures price changes over time for a productID, which allows us to generate historical price charts
    - productID
    - timestamp
    - price

- User:
  - This entity represents a user of the service, containing their contact info and preferences for receiving price drop notifications
    - userID
    - email
    - preferences: { settings object containing notification delivery methods, formats, etc }

## API design

- Note that we’ll use userID if the user is authenticated. The userID could be placed in a session token or JWT in the Authentication header as such:
  - Authentication: Bearer

- The below caching header could also be applied to fetch frequently accessed entries quicker. “no-cache” means that a browser may cache a response, but it must first send a validation request to an origin server before caching it. “Public” means that the entry can be cached by any intermediary such as a proxy server between the client and server - which is beneficial for static content like images and stylesheets. “Private” means that an entry can only be cached by the browser and no other intermediary. This is important for private content:
  - Cache-Control: no-cache max-age Private/Public

### Retrieve price history for charts

- This endpoint will retrieve the historical price data for a productID, within a time range and using an interval (daily, weekly, monthly, etc). By using an interval / granularity, we can return averages over the interval, which will be used to create the chart
- The price history will be retrieved from the price history entity.

Request:
```bash
GET /products/:productID/history?startTime&endTime&interval
```

Response:
```bash
{
  history: [ { timestamp, price } ]
}
```

### Subscribe to price drop notifications

- Users can subscribe to price drop notifications for a specific threshold using this endpoint. An entry in the subscriptions entity will be created

Request:
```bash
POST /subscriptions
{
  productID,
  threshold,
  notificationType: EMAIL / SMS
}
```

## Workflow

Before diving into the design, let's understand the high level workflow. The workflow shows how information moves from initial collection to final user output. Understanding this flow early in our design process serves multiple purposes. First, it helps ensure we're aligned with our interviewer on the core functionality before getting into implementation details. Second, it provides a clear roadmap that will guide our high-level design decisions. Finally, it allows us to identify potential bottlenecks or issues before we commit to specific architectural choices.

In this case, the "hidden" requirement is that we need to be able to get the data in the first place!

1. Web crawlers and Chrome extension collect current prices from Amazon product pages
2. Price data is validated and stored in our price history table
3. If the backend servers determine that prices have changed, then it will send notifications to users if price drops below threshold

Note: Note that this is simple, we will improve upon as we go, but it's important to start simple and build up from there.

## HLD

Note: Amazon is not friendly to price tracking services. They don't provide an API, actively discourage scraping, and implement rate limiting (typically 1 request per second per IP address). We must design our system to work within these constraints while remaining respectful of Amazon's terms of service.

Requirements:

### 1. View price history (via website or Chrome extension)

Users want to see how prices have changed over time for specific Amazon products. This serves as the foundation of our entire system and breaks down into two distinct challenges: acquiring price data from Amazon and serving historical price queries efficiently. For the first iteration, we'll establish the basic data collection and storage infrastructure. Our architecture needs a web crawler to gather price data, a storage layer for historical prices, and an API service to serve price history to users.

We'll separate the Price History service from the crawler because they have different scaling characteristics. The history service needs to handle many concurrent read requests from users viewing charts, while the crawler operates on a scheduled basis with consistent write patterns. This separation also allows us to optimize each service independently.

Because the Price history service will also make frequent requests to the Price DB and compute the aggregated price history using the interval / granularity, we can utilize an ECS cluster / service running ECS tasks (containers to handle requests for price history) on compute or IO / memory optimized EC2 instances.

As for the databases, we'll make a strategic separation based on scale and access patterns. Our Primary DB will store products and subscriptions together since these have similar operational characteristics - they're relatively small and have traditional CRUD access patterns. However, we'll put our Price DB completely separate because price history data has fundamentally different requirements: it will grow to billions of rows, is append-only, requires time-series optimizations, and can tolerate eventual consistency. This separation allows us to optimize each database independently and scale them based on their specific needs.

Also, because the Price DB will need to be frequently updated, and is append only, we can likely use a high write intensive DB like Cassandra, which is perfect for this use case. The Cassandra schema could also look as follows, where the primary key will be the productID, while the clustering key will be the time, so that a partition is ordered by the time field:

```cql
create table priceHistory (
  productID UUID,
  time timestamp,
  primary key (productID)
) with clustering order by (time asc)
```

The Primary DB also will handle usual CRUD requests by the web crawler and other services like the Notification service, thus either Postgres or DynamoDB should work well here. If we use DynamoDB, we can likely use LSIs on the Products table, where the updatedTime could be the sort key, allowing the crawler to fetch productIDs which haven't been updated recently. Also, if using both Postgres or DynamoDB, we can create additional indexes (or GSIs) on frequently accessed fields which can further help to categorize the products into categories like tech, clothing, etc.

We'll choose to "black box" the web crawler service for now. We can assume it periodically scrapes Amazon product pages and updates the Price table. Of course, we'll go into more detail about just how this might be accomplished in our DD. For now, we can assume that the web crawler will also be run on multiple IO / memory optimized EC2 instances since it needs to make multiple network and DB requests.

The workflow for requesting the price history (via the website or Chrome extension) is as follows:
1. Client requests the endpoint 'GET /products/:productID/history'
2. The Price History service queries the Price DB using the productID and time range
3. The Price History service will then return the historical prices as JSON, ready to be rendered via the website or Chrome extension as a chart

![Price History Workflow](price_tracking_images/1.png)

### 2. Subscribing to price drop notifications with thresholds (via website or Chrome extension)

Building on our price history foundation, users now want to receive notifications when product prices drop below their specified thresholds. This introduces new requirements for user management, subscription storage, and notification delivery. We'll add new components while leveraging our existing infrastructure. For our initial implementation, we can start with a simple every 1-2 hr polling approach that gets the job done (likely via an Eventbridge schedule and lambda), knowing we can optimize this later in the DD.

This requirement should now use:
- Subscription service:
  - This service saves productID subscriptions and threshold settings in the subscriptions table
- Notification cron job:
  - Periodically checks for price changes using the Primary and Price DB, and sends email notifications (likely using a lambda that sends an SES email)

We'll separate the Subscription service from the Price History service because they serve different user actions and have different data access patterns. The Subscription service handles user account operations (create, update, delete subscriptions) which are not as frequent, while the Price History service focuses on read-heavy price queries.

For our initial Notification service, we have a straightforward batch processing approach:
1. Cron job runs every 1-2 hours to check for productID price changes within the past 1-2 hours
2. For each price change, the Notification service will query the subscriptions table from the Primary DB to find the subscriptionIDs and its notification preferences
3. The Notification service will send notifications for any triggered subscriptions. The Notification service could also mark the subscriptionID as being SENT with a updatedPrice field to avoid making duplicate notifications for the same updatedPrice

Note: This polling approach works but has obvious limitations. Users might wait up to 1-2 hours for price drop notifications, and we're doing expensive DB scans every few hours. We'll explore much better real-time approaches in the DD that provide notification delivery within minutes from price change.

The workflow will be as follows when a user subscribes to price alerts:
1. User requests the endpoint 'POST /subscriptions'
2. The Subscription service will create an entry in the subscriptions table using the userID, productID, threshold and notification preferences

This simple approach makes sure we have a working subscription system that satisfies our functional requirements. Users get price drop alerts (just not immediately), and our system remains easy to understand and debug. The periodic nature also prevents notification processing from interfering with our core price tracking functionality.

![Subscription Workflow](price_tracking_images/2.png)

<br/>
<br/>

### Client

- The client is either the website or the Chrome extension. In either case, they're how the user views the price history charts

### API gateway

- The API gateway will handle authentication, rate limiting and route requests to appropriate services

### Price history service

- The Price history service will retrieve the price history from the Price DB, and return the data needed to create the charts

### Web crawler

- The Web crawler will crawl the Amazon product pages to collect current prices

### Price database

- The Price DB will store the price history table which contains the frequently updated historical price data

### Primary database

- The primary DB will store all other tables aside from the price history, such as the products and subscriptions tables

### Subscriptions service

- The Subscriptions service will allow users to create subscriptions for price alerts via the endpoint 'POST /subscriptions'

### Notification service

- The Notification service will request the Primary and Price DB via 1-2 hr polling in a cron job. It will find the price changes for productIDs, and send notifications for price changes if they have a subscriptionID. We'll discuss in the DD how we can deliver notifications within minutes from the productID's price change.

<br/>
<br/>

![High-Level Design](price_tracking_images/3.png)

## DD

### Efficiently discovering and tracking 500M Amazon products

Up until now, we've been "black boxing" our web crawler, simply assuming it somehow gets price data from Amazon and updates our database. But when we confront the reality of discovering and tracking 500M products while respecting Amazon's rate limiting constraints, we realize this is actually one of our biggest technical challenges.

We're solving two distinct but related problems here:
- The first challenge is product discovery:
  - We need to find all 500M existing products when we launch, plus discover the approximately 3k new products Amazon adds daily.
- The second challenge is price monitoring:
  - We need to efficiently update prices for millions of known products while prioritizing which ones to check most frequently.

Both challenges face the same fundamental constraint of Amazon's rate limiting (1 request per second per IP), but understanding them separately helps us design more effective solutions. We can use the following solutions to handle product discovery and monitoring:
- Native web crawling approach
- Prioritized crawling based on user interest
- Chrome extension + selective crawling
  - It's important to note that while the client's Chrome extension will send the Price Update service updates on the productIDs, current prices and page metadata, our current web crawler will still crawl Amazon for products that haven't been viewed by extension users recently. By still using a crawler, we can use a priority queue to prioritize which popular and recently "not-crawled" productIDs to crawl, likely with an SQS delayed message FIFO queue.
  - This hybrid approach solves our scale challenge by turning our biggest constraint (the need to monitor millions of products) into our competitive advantage through crowdsourced data collection. Instead of fighting Amazon's rate limits with massive crawler infrastructure, we leverage user behavior to naturally prioritize and collect data for the products that matter most.

Note: If you're thinking to yourself, "How the hell would I know this? I know nothing about Chrome extensions!" don't sweat it. When I ask this in an interview, I usually lead with, "Your extension is installed on millions of browsers actively viewing product pages. Can you make use of that somehow?"

Also, note that instead of having a scheduler within the crawler as shown below, the Price Update service could directly queue the crawl request into the frontier queue using SQS's delayed message feature (for assigning a crawl request priority) to validate the price change. Additionally, to further ensure ordering of price changes for similar productIDs, we can add a messageGroupID where the messages in the messageGroupID will be ordered.

![Crawler Architecture](price_tracking_images/4.png)

### Handling potential malicious price updates from Chrome extension users

The Chrome extension approach from our previous deep dive creates a powerful data collection advantage, but it introduces a critical reliability challenge. With 1M users potentially submitting price data, we must assume that some percentage will be malicious actors, mistaken users, or systems experiencing technical issues.

Consider this scenario: A malicious user reports that the latest iPhone costs $0.01, triggering price drop notifications to thousands of subscribers. Users rush to Amazon only to find the normal price, damaging our credibility and user trust. Our notification system's effectiveness depends entirely on data accuracy, making this challenge essential to solve properly. We can solve this using the following:
- Consensus based validation
  - In order to assign a "trust score" to users of the website / extension, we can likely reference their userID, email or IP which they used to subscribe to prices, or search for productIDs.
- Trust-but-verify with priority verification
  - Likewise with the previous approach, we can assign a trust score to the userID, email or IP, such that for less trustworthy extension users, we'll queue the price update into the crawler's frontier queue with a high priority score. The crawler will then verify the price update's validity before it saves the update in the Price DB.
  - This trust-but-verify approach strikes a really nice balance, providing immediate user value while maintaining the data integrity essential for long-term trust.

### Efficiently processing price changes and notifying subscribed users 

Right now, we're using a straightforward cron job that runs every 1-2 hrs to scan for price changes and send notifications. While this might work for basic functionality, it fails to meet our non-functional requirement of delivering notifications within 1 hr of price changes. More critically, the polling approach creates massive database load as we scale to millions of price updates and subscriptions.

The Notification service's effectiveness directly impacts user experience. Users expect timely notifications for price drops, especially for competitive deals that might sell out quickly. A 1-2 hr delay often means missing time-sensitive opportunities, making our service less valuable than competitors with faster notification systems.

The fundamental problem with our current polling approach is that we're asking the wrong question at the wrong time. Instead of asking "what changed in the last 1-2 hrs?" every 1-2 hrs, we should be asking "who cares about this change?" immediately when a change happens.

Moving to event-driven notifications is straightforward. When a price changes, we immediately know which users need alerts without expensive DB scans. We have 2 solid ways to implement this, each with different trade-offs.

#### CDC to Kafka's Price Change stream, to trigger Price change workers to send notifications

The first option is database change data capture (CDC), where we configure our Price DB to automatically publish events whenever price data changes. Different providers of Cassandra, such as DataStax offers their own implementation of CDC which we could utilize. This CDC feature also allows us to publish messages into a Kafka topic. Refer to the resource notes 'CDC' for how Cassandra's CDC to Kafka can be setup.

When the Price Update service or crawler workers insert new price records into the Price DB, the Price DB sends messages to Kafka (or similar) containing the productID, old price, and new price. Our Notification service's Price Change workers will subscribe to multiple topics corresponding to the productIDs, where a single topic such as "laptops" could contain multiple productIDs. Upon consuming the messages, the Price Change workers will query the Subscriptions table to find affected users, and then send them the notifications.

This is clean because price changes automatically trigger notifications without any direct polling or monitoring. The workflow for triggering notifications becomes completely automatic and dependent on price changes.

![CDC to Kafka Workflow](price_tracking_images/5.png)

#### Price Update service / crawler writes to both Price DB and publishes messages to Kafka's Price Change stream (Notification service's Price change workers consume from Kafka and sends notifications to subscribers)

The second option is dual writes, where the Price Update service and crawler writes to both the Price DB and publishes messages to Kafka simultaneously. When crawler data or extension updates from the Price Update service come in, the services (Price Update service and crawler) that writes to the Price DB also publishes messages to Kafka. Then, the Notification service's Price Change workers will consume price changes from the Kafka topics and send notifications to subscribed users for said price changes.

The dual-write approach lets us be smarter about which changes trigger notifications. We can filter out tiny price fluctuations that wouldn't interest users or batch multiple rapid changes before publishing messages using Kafka's batch size and interval producer properties. We also avoid the overhead of DB triggers on every single price update.

Regardless of which option we choose, the Notification service's Price change workers (Kafka consumers) work the same way. They read price change messages from Kafka and immediately query the Subscriptions table to find users whose thresholds have been triggered. For each matching subscription, we send an email notification through our existing Notification service logic, which will be in the Price Change worker.

The beauty is that we can send messages into Kafka for only notable price changes. Most products don't change price very frequently, so we're talking about a manageable number of messages even at scale. When a popular productID's price drops, we'll run one query to find all subscribers with thresholds above the new price and send those notifications right away via the Price Change workers.

### Serving fast price history queries for chart generation

To delivery a < 500 ms latency on price history reads and chart generation requests, and also to support a very high write load, we'll need to optimally select the Price DB.

Consider the scale challenge that comes from aggregating price data for popular productIDs. Popular productIDs might have price data points every few hours for several years, resulting in many thousands of records per product. When users request a 2 year price chart, we must aggregate this data into appropriate time buckets (daily or weekly averages) and return it quickly enough for smooth user experience. Raw database queries struggle with this aggregation workload.

To optimally select the Price DB, we could use the following:
- Scheduled pre-aggregation with cron jobs
  - By using this approach, when new price updates come into the Price DB, the pre-aggregated data may become stale and thus require the cron job to be re-run to update the pre-aggregated data. However, because this is still an aggregation, this staleness may be acceptable since users are looking at historical trends using this feature rather than real-time price changes.
- TimescaleDB for real-time price analytics
  - The TimescaleDB approach provides the performance and flexibility needed for production-scale price chart serving while maintaining the real-time responsiveness users expect. By choosing the right tool for analytical workloads, we achieve both simplicity and performance without complex caching or pre-aggregation strategies.
  - However, one down-side of using this approach is that TimescaleDB is still an extension of Postgres, and thus the underlying Postgres Price DB may not be able to handle a very large write load for 500M productIDs, like with Cassandra.
- Cassandra (our selected Price DB)
  - Overall, we could use either TimescaleDB or Cassandra for the Price DB. While Cassandra does have a better write load, TimescaleDB will be better in returning price history over different intervals / granularities because it provides built-in features to query aggregations quickly, whereas Cassandra will use the clustering keys to return ordered aggregated data.
  - However, if using Cassandra, instead of aggregating the data ourselves, we could create multiple tables containing different aggregation granularities (daily, weekly, monthly) such that when price changes come in, the updates will be made to all of the Cassandra tables accordingly. This is actually how Cassandra is meant to be used, where we create tables for the specific aggregation queries themselves (daily, weekly, monthly), instead of aggregating the data in our own CQL query.

A CQL query on the Price DB's daily table might look as follows:

```cql
select time, price from priceHistoryDaily
where productID = :productID and time between :startTime and :endTime
```

### Scaling the components

#### Stateless services (Price Update / Price History / Subscriptions / Notification service's Price Change workers / Crawler workers)

All of the stateless services, such as the Price Update, Price History and Subscriptions services will likely run multiple EC2 instances within an ASG behind an ALB. Because price updates and price history requests may be more frequent, we can easily horizontally scale the Price Update and Price History services.

Both the Price Change workers and Crawler workers will have the similar responsibility of polling a queue (Kafka and SQS respectively) to process the price changes and crawling requests respectively. 

Because of this, the Price Change workers will likely be consumer workers (running within ECS tasks) in multiple consumer groups, where we can scale additional consumer workers when price changes occur more frequently. Also, to handle spikes in Kafka, we can configure a batch size / interval and buffer size / interval in which we can temporarily buffer messages to give more time for consumer workers to process them. We can also increase the polling wait time within the consumer workers to ensure spikes don't cause a SPOF.

The Crawler workers will also likely be ECS tasks which will poll the SQS queue (frontier queue) for crawler requests. If using ECS, we can also scale both the number of ECS tasks and EC2 instances using ECS service's auto-scaling and an ASG respectively. By using an ECS service's auto-scaling feature, we can scale the number of ECS tasks using Cloudwatch metrics like SQS queue size, CPU usage, etc.

#### Price DB

As mentioned before, we can scale the Cassandra Price DB by partitioning the table's rows using time as the clustering key. We can also create multiple tables for different aggregation granularities to improve the read load on the price history chart generation.

#### Primary DB

Because the Primary DB is storing the products and subscriptions tables, we can likely shard the products by their category. This way, because it's likely for similar products belonging to the same category to drop in price (during sales), queries on the products table for multiple productIDs for the same category could go to the same shard.

We could also likely shard the subscriptions table by the userID, such that subIDs for the same userID are all in the same shard. However, some users might have multiple subIDs, which might cause an uneven distribution across the shards. To handle this, our service could limit the number of subscriptions according to different user premium membership subscriptions.

#### Price Change stream (Kafka)

We mentioned that we're using Kafka as the Price Change stream, where the Price Update service and Crawler's workers will publish price changes into the Kafka topics. It's important to note that we can't have a single topic per productID. While a Kafka cluster can have an unlimited number of topics, it's still limited by the broker hardware's storage. We can likely maintain 500k - 1M topics in a largescale Kafka cluster that's running across multiple storage optimized EC2 instances. Therefore, a topicID would instead need to reference multiple productIDs using some categoryID, like "laptops" or something more specific like "mac-laptops".

By having a topic reference a categoryID, there will be less infrequently accessed topics because a topic != productID. The productID to topic distribution also won't be as sparse. Lastly, to ensure price updates for the same productID come in order, we can partition a topic using a more granular category such as "mac-laptops-14-inch". This way, a topic's partition will have ordered price change messages which the Price Change workers can safely consume from.

<br/>
<br/>

Taking a step back, we've designed and scaled a system that can intelligently collect price data from Amazon, validate it, and notify users when prices drop all while serving price history charts to users in real-time.












