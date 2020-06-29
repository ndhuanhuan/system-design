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
https://www.mongodb.com/scale/nosql-vs-relational-databases#:~:text=or%20service%20interruptions.-,Data%20Structure,clearly%20defined%20by%20their%20relationships.
SQL databases are known as relational databases, and have a table-based data structure, with a strict, predefined schema required. NoSQL databases, or non-relational databases, can be document based, graph databases, key-value pairs, or wide-column stores. NoSQL databases don’t require any predefined schema, allowing you to work more freely with “unstructured data.” Relational databases are vertically scalable, but usually more expensive, whereas the horizontal scaling nature of NoSQL databases is more cost-efficient.
 
- A key-value store is an alternative construction, where each element has a unique key corresponding with some chunk of data as the store.
- Technically speaking the "value" in key-value can be any abstract type, from a string all the way to binary blob. The document model is a subtype of key-value stores in which each of the data chunks is forced to be a document.
- An example of a key-value store is Redis. An example of a document-oriented database (a database using the document model) is MongoDB.
- The graph model is a data model that treats data as blobs of information connected by edges representing relationships.
- The biggest issue with the relational model that so-called "NoSQL" models try to address is object-impedence mismatch. The structure of the data in the database doesn't look that much like the structure of the data as it is applied in the program, forcing the programmer to have to deal with two different memory structures.
