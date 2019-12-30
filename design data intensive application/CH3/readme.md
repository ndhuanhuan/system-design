https://blog.csdn.net/weixin_33778544/article/details/90781715
https://blog.csdn.net/weixin_33721344/article/details/86862861
log-structured storage engines, and page-oriented storage engines
P71

In order to efficiently find the value for a particular key in the database, we need a
different data structure: an index.

An index is an additional structure that is derived from the primary data. Many databases
allow you to add and remove indexes, and this doesn’t affect the contents of the
database; it only affects the performance of queries.

This is an important trade-off in storage systems: well-chosen indexes speed up read
queries, but every index slows down writes.

you—the application developer or database
administrator—to choose indexes manually, using your knowledge of the application’s
typical query patterns.

P72
- Hash Indexes


Let’s say our data storage consists only of appending to a file, as in the preceding
example. Then the simplest possible indexing strategy is this: keep an in-memory
hash map where every key is mapped to a byte offset in the data file—the location at
which the value can be found.

=> suit for
A storage engine like Bitcask is well suited to situations where the value for each key
is updated frequently. For example, the key might be the URL of a cat video, and the
value might be the number of times it has been played (incremented every time
someone hits the play button). In this kind of workload, there are a lot of writes, but
there are not too many distinct keys—you have a large number of writes per key, but
it’s feasible to keep all keys in memory.



P74
Each segment now has its own in-memory hash table, mapping keys to file offsets. In
order to find the value for a key, we first check the most recent segment’s hash map;
if the key is not present we check the second-most-recent segment, and so on. The
merging process keeps the number of segments small, so lookups don’t need to check
many hash maps.



real implementation considerations are:
-- File format
-- Deleting records
-- Crash recovery
-- Partially written records
-- Concurrency control


P76
- SSTables and LSM-Trees

Now we can make a simple change to the format of our segment files: we require that the sequence of key-value pairs is sorted by key.

1.
Merging segments is simple and efficient, even if the files are bigger than the
available memory.
 you start reading the input files side by side, look
at the first key in each file, copy the lowest key (according to the sort order) to the output file, and repeat. This produces a new merged segment file, also sorted by key.

2.
You still need an in-memory index to tell you the offsets for some of the keys, but it can be sparse: one key for every few kilobytes of segment file is sufficient, because a few kilobytes can be scanned very quickly

3.
it is possible to group those records into a block and compress it
before writing it to disk

P78
Constructing and maintaining SSTables
