# Data Models and Query Language

## Relational Model

Good at handling:

* Structural relationship, between one-to-many and many-to-many
* Make the data relationship explicit (schema-on-write)
* SQL

## Graph Model

Target use case: anything is potentially related to anything.

Good at handling:

* Many-to-many, interconnected, relationship
* Graph traverse

### Property Graph Model

* Vertex
  * A unique identifier
  * A set of outgoing edges
  * A set of incoming edges
  * A collection of properties (key-value pairs)
* Edge
  * A unique identifier
  * The vertex at which the edge starts (the tail vertex)
  * The vertex at which the edge ends (the head vertex)
  * A collection of labels to describe the kind of relations between two vertices
  * A collection of properties (key-value pairs)

### Triple Model

Much similar with the property graph model

* Subject
* Predicate
* Object

Using by SPARQL and is base of the semantic network


# Notes
- A key-value store is an alternative construction, where each element has a unique key corresponding with some chunk of data as the store.
- Technically speaking the "value" in key-value can be any abstract type, from a string all the way to binary blob. The document model is a subtype of key-value stores in which each of the data chunks is forced to be a document.
