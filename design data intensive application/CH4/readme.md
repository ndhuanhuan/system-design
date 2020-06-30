# Encoding and Evolution

## Important Points:

* The implementation of Protocol Buffers (p117)
* Message-passing dataflow (p139)

## Important Care in Data Encoding:

* Backwards/forwards compatibility
* Rolling update, which means nodes can running different version application at the send time. It must ensure the new version application can read old data and old application can read new version data with no problem.
* Benefit: update without downtime, small but frequent update, partial update for A/B test

# Notes
## Human-readable data interchange formats
- JSON and XML (and CSV, and the other usual suspects) are common data interchange formats, meant to be moved between application boundaries.
- These formats are considered to be lowest common denominators, however.
- They have parsing problems. For example, it's often impossible to difficult to determine the type of an object.
- Being human-readable, they are also inefficient in resource terms when performing network transfers.
- Still, for simple use cases these formats are usually sufficient.
