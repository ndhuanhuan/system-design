---
title: Intro to Spring Boot
created: '2019-12-30T01:58:33.242Z'
modified: '2019-12-30T06:58:18.456Z'
---

# Intro to Spring Boot

## Spring boot
### The magic @SpringBootApplication annotation

- It enables component scanning
- The application class itself becomes a configuration class
- It enables autoconfiguration, where Spring Boot looks for JAR files in the classpath that it can configure automatically. If you, for example, have Tomcat in the classpath, Spring Boot will automatically configure Tomcat as an embedded web server.

## Spring WebFlux
Support two different programming models:
- An annotation-based imperative style, similar to the already existing web framework, Spring Web MVC, but with support for reactive services
- A new function-oriented model based on routers and handlers

## Exploring SpringFox
SpringFox is an open-source project, separate from the Spring Framework, that can create Swagger-based API documentation at runtime.

## Understanding Spring Data
Spring Data comes with a common programming model for persisting data in various types of database engine.

## Understanding Spring Cloud Stream
Spring Cloud Stream. Spring Cloud Stream provides a streaming abstraction over messaging, based on the publish-and-subscribe integration pattern. Spring Cloud Stream currently comes with support for Apache Kafka and RabbitMQ out of the boxSpring Cloud Stream. 

## Summarize
Spring Boot is used to simplify the development of Spring-based, production-ready applications. It is strongly opinionated in terms of how to set up both core modules from the Spring Framework and third-party products.

Spring WebFlux is a new module in the Spring family and is used to develop reactive, that is, non-blocking, REST services. It runs on both lightweight web servers such as Netty and on any Servlet 3.1+ compatible web server. It also supports the programming model from the older Spring MVC module; it is easy to move REST services written for Spring MVC to Spring WebFlux without fully rewriting the code.

SpringFox can be used to create Swagger and OpenAPI-based documentation regarding REST services. It creates the documentation on the fly at runtime by inspecting the annotations for the REST services – both the Spring annotations and some Swagger specific annotations – if used.

Spring Data provides an elegant abstraction for accessing and manipulating persistent data using entities and repositories. The programming model is similar, but isn't portable between different types of database, for example, relational, document, key-value, and graph databases.Spring Boot is used to simplify the development of Spring-based, production-ready applications. It is strongly opinionated in terms of how to set up both core modules from the Spring Framework and third-party products.

Spring Cloud Stream provides a streaming abstraction over messaging, based on the publish and subscribe integration pattern. Spring Cloud Stream comes with out of the box support for Apache Kafka and RabbitMQ but can be extended to support other messaging brokers using custom binders.

Docker makes the concept of containers as a lightweight alternative to virtual machines easy to use. Based on Linux Namespaces and Control Groups, containers provide isolation similar to what traditional virtual machines provide, but with a significantly lower overhead in terms of CPU and memory usage. Docker is a very good tool for development and testing but in most cases requires a container orchestrator such as Kubernetes to be used in a production environment.
