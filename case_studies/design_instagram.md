**Instagram** is a social networking service, which enables its users to upload and share their pictures and videos with other users. Users can share either publicly or privately, as well as through a number of other social networking platforms, such as Facebook, Twitter, Flickr, and Tumblr.

## Functional Requirements

### Functional​ ​Requirements
1. Users​ ​should​ ​be​ ​able​ ​to​ ​upload/download/view​ ​photos.
2. Users ​​can ​​perform ​​searches ​​based ​​on ​​photo/video​​titles.
3. Users​​ can​ ​follow​ ​other ​​users.
4. The ​​system ​​should ​​be ​​able ​​to ​​generate ​​and ​​display ​​a ​​user’s ​​timeline ​​consisting ​​of ​​top​ ​photos​ ​from​​ all​ ​the ​​people ​​the​ ​user ​​follows.

### Non-functional​ ​Requirements
1. Our​ ​service​ ​needs​ ​to​ ​be​ ​highly​ ​available.
2. The​​ acceptable​ ​latency​ ​of ​​the ​​system​​ is ​​200ms​ ​for​​ timeline ​​generation.
3. Consistency​ ​can​ ​take​ ​a​ ​hit​​(in​​ the ​​interest​ ​of​ ​availability),​ ​if​​ a​ ​user ​​doesn’t ​​see​ ​a ​​photo ​​for ​​a​ ​while, ​​it ​​should ​​be ​​fine.
4. The​​ system​​ should​ ​be​ ​highly ​​reliable,​ ​any​ ​photo/video ​​uploaded ​​should​ ​not ​​be ​​lost.

## Some Design Considerations
The system would be read-heavy, so we will focus on building a system that can retrieve photos quickly.

* Practically users can upload as many photos as they like. Efficient management of storage should be a crucial factor while designing this system.
* Low latency is expected while reading images.
* Data should be 100% reliable. If a user uploads an image, the system will guarantee that it will never be lost.

## Important Read
https://www.scribd.com/document/89025069/Mike-Krieger-Instagram-at-the-Airbnb-tech-talk-on-Scaling-Instagram

## Capacity​ ​Estimation​ ​and​ ​Constraints
1. Let’s​ ​assume​ ​we​ ​have​ ​300M​ ​total​ ​users,​ ​with​ ​1M​ ​daily​ ​active​ ​users.
2. 2M​ ​new​ ​photos​ ​every​ ​day,​ ​23​ ​new​ ​photos​ ​every​ ​second.
3. Average​ ​photo​ ​file​ ​size​ ​=>​ ​200KB
4. Total​ ​space​ ​required​ ​for​ ​1​ ​day​ ​of​ ​photos. `2M​ ​*​ ​200KB​ ​=>​ ​400​ ​GB`
5. Total​ ​space​ ​required​ ​for​ ​5​ ​years: `400GB​ ​*​ ​365​ ​(days​ ​a​ ​year)​ ​*​ ​5​ ​(years)​ ​~=​ ​712​ ​TB`

## High Level System Design
At a high-level, we need to support two scenarios, one to upload photos and the other to view/search photos. Our service would need some block storage servers to store photos and also some database servers to store metadata information about the photos.

![insta design](https://user-images.githubusercontent.com/6800366/37567179-38a5655a-2ae9-11e8-9fd6-5b6aa7547d93.png)

## Database Schema
* We need to store data about users, their uploaded photos, and people they follow. Photo table will store all data related to a photo, we need to have an index on (PhotoID, CreationDate) since we need to fetch recent photos first.

![insta db](https://user-images.githubusercontent.com/6800366/37567382-2ea79232-2aec-11e8-988c-48b7b35fa31e.png)

One simple approach for storing the above schema would be to use an RDBMS like MySQL since we require joins. But relational databases come with their challenges, especially when we need to scale them. For details, please take a look at SQL vs. NoSQL.

* We can store photos in a distributed file storage like HDFS or S3.

* We can store the above schema in a distributed key-value store to enjoy benefits offered by NoSQL. All the metadata related to photos can go to a table, where the ‘key’ would be the ‘PhotoID’ and the ‘value’ would be an object containing PhotoLocation, UserLocation, CreationTimestamp, etc.

* We also need to store relationships between users and photos, to know who owns which photo. Another relationship we would need to store is the list of people a user follows. For both of these tables, we can use a wide-column datastore like Cassandra. For the ‘UserPhoto’ table, the ‘key’ would be ‘UserID’ and the ‘value’ would be the list of ‘PhotoIDs’ the user owns, stored in different columns. We will have a similar scheme for the ‘UserFollow’ table.

* Cassandra or key-value stores in general, always maintain a certain number of replicas to offer reliability. Also, in such data stores, deletes don’t get applied instantly, data is retained for certain days (to support undeleting) before getting removed from the system permanently.

## Component Design
* Writes or photo uploads could be slow as they have to go to the disk, whereas reads could be faster, especially, if they are being served from cache.

* Uploading users can consume all the connections, as uploading would be a slower process. This means reads cannot be served if the system gets busy with all the write requests. To handle this bottleneck we can split out read and writes into separate services. **Since web servers have connection limit**, we should keep this thing in mind before designing our system. Let’s assume if a web server can have maximum 500 connections at any time, which means it can’t have more than 500 concurrent uploads or reads. This guides us to have separate dedicated servers for reads and writes so that uploads don’t hog the system.

* Separating image read and write requests will also allow us to scale or optimize each of them independently.

![insta comp design](https://user-images.githubusercontent.com/6800366/37567408-a2e7e048-2aec-11e8-9247-5b589534b1b3.png)

## Reliability and Redundancy
* Losing files is not an option for our service. Therefore, we will store multiple copies of each file, so that if one storage server dies, we can retrieve the image from the other copy present on a different storage server.

* This principle also applies to other components of the system. If we want to have high availability of the system, we need to have multiple replicas of services running in the system. So that if a few services die down, the system is still available and serving. Redundancy removes the single point of failures in the system.

* If only one instance of a service is required to be running at any point, we can run a redundant secondary copy of the service that is not serving any traffic but whenever primary has any problem it can take control after the failover.

* Creating redundancy in a system can remove single point of failure and provide a backup or spare functionality if needed in a crisis. For example, if there are two instances of the same service running in production, and one fails or degrades, the system can failover to the healthy copy. Failover can happen automatically or require manual intervention.

![insta replicas](https://user-images.githubusercontent.com/6800366/37567469-788c828a-2aed-11e8-8b16-60c9a7b0ca67.png)

## Data Sharding
Different schemes for metadata sharding:

* **Partitioning based on UserID:** Let’s assume we shard based on the UserID so that we can keep all photos of a user on the same shard. If one DB shard is 4TB, we will have 712/4 => 178 shards. Let’s assume for future growths we keep 200 shards.

    So we will find the shard number by UserID % 200 and then store the data there. To uniquely identify any photo in our system, we can append shard number with each PhotoID.

    How can we generate PhotoIDs? Each DB shard can have its own auto-increment sequence for PhotoIDs, and since we will append ShardID with each PhotoID, it will make it unique throughout our system.

    **What are different issues with this partitioning scheme?**

    How would we handle hot users? Several people follow such hot users, and any photo they upload is seen by a lot of other people.
    Some users will have a lot of photos compared to others, thus making a non-uniform distribution of storage.
    What if we cannot store all pictures of a user on one shard? If we distribute photos of a user onto multiple shards, will it cause higher latencies?
    Storing all pictures of a user on one shard can cause issues like unavailability of all of the user’s data if that shard is down or higher latency if it is serving high load etc.

* **Partitioning based on PhotoID:** If we can generate unique PhotoIDs first and then find shard number through PhotoID % 200, this can solve above problems. We would not need to append ShardID with PhotoID in this case as PhotoID will itself be unique throughout the system.

**How can we generate PhotoIDs?** Here we cannot have an auto-incrementing sequence in each shard to define PhotoID since we need to have PhotoID first to find the shard where it will be stored. One solution could be that we dedicate a separate database instance to generate auto-incrementing IDs. If our PhotoID can fit into 64 bits, we can define a table containing only a 64 bit ID field. So whenever we would like to add a photo in our system, we can insert a new row in this table and take that ID to be our PhotoID of the new photo.

**Wouldn’t this key generating DB be a single point of failure?** Yes, it will be. A workaround for that could be, we can define two such databases, with one generating even numbered IDs and the other odd numbered. For MySQL following script can define such sequences:

```
KeyGeneratingServer1:
auto-increment-increment = 2
auto-increment-offset = 1

KeyGeneratingServer2:
auto-increment-increment = 2
auto-increment-offset = 2
```
We can put a **load balancer** in front of both of these databases to round robin between them and to deal with down time. Both these servers could be out of sync with one generating more keys than the other, but this will not cause any issue in our system. We can extend this design by defining separate ID tables for Users, Photo-Comments or other objects present in our system.

Alternately, we can implement a key generation scheme.

**How can we plan for future growth of our system?** We can have a large number of logical partitions to accommodate future data growth, such that, in the beginning, multiple logical partitions reside on a single physical database server. Since each database server can have multiple database instances on it, we can have separate databases for each logical partition on any server. So whenever we feel that a certain database server has a lot of data, we can migrate some logical partitions from it to another server. We can maintain a config file (or a separate database) that can map our logical partitions to database servers; this will enable us to move partitions around easily. Whenever we want to move a partition, we just have to update the config file to announce the change.

## Cache and Load balancing
* To serve globally distributed users, our service needs a massive-scale photo delivery system. Our service should push its content closer to the user using a large number of geographically distributed photo cache servers and use CDNs (for details see Caching).

* We can introduce a cache for metadata servers to cache hot database rows. We can use Memcache to cache the data and Application servers before hitting database can quickly check if the cache has desired rows. Least Recently Used (LRU) can be a reasonable cache eviction policy for our system. Under this policy, we discard the least recently viewed row first.

**How can we build more intelligent cache?** If we go with 80-20 rule, i.e., 20% of daily read volume for photos is generating 80% of traffic which means that certain photos are so popular that the majority of people reads them. This dictates we can try caching 20% of daily read volume of photos and metadata.


# Refs
https://www.codercrunch.com/design/634265/designing-instagram
