# Introduction 

Example Qt 6 project structure demonstrating the implementation of Clean Architecture principles for maintainable and scalable software development. 

The basic concepts come from the book *Clean Architecture: A Craftsman's Guide to Software Structure and Design* by Robert C. Martin and the teachings of Bernardo Estacio Abreu, Software Architect. This proof of concept was initiated so as to offer a new base for the software Skribisto.

This example is minimal but functional and tested.

Main points:
- one simple entity: Author
- a very basic QWidget UI 
- asynchronous API
- asynchronous undo redo system
- automapper
- Command and Query Responsibility Segregation ([CQRS](https://learn.microsoft.com/en-us/azure/architecture/patterns/cqrs))
- use of Data Transfert Objects ([DTO](https://www.baeldung.com/java-dto-pattern))
- file load and save



# Structure

## Core

Business side of the clean architecture.

### Domain

Contains the entities. Simple inert QObjects exposing their fields using Q_PROPERTY

### Persistence

#### Database

Internal database. Here a simple SQLite.

#### Repositories

Agnostic wrapper around the database. One repository per entity.
### Contracts

Used to enforce the [Interface Segregation Principle](https://stackify.com/interface-segregation-principle/) and the [ependency Inversion Principle](https://stackify.com/dependency-inversion-principle/). Also contains DTOs and CQRS implementation which must be usable to all parts. All parts, but domains, depend of this library.

### Infrastructure

Here lie the connections of to the external services, like loading and saving files.

### Application

Use cases. All the business logics.

## GUI

GUI side of the clean architecture.

### Presenter

Each feature offer its own Controller singleton such as AuthorController or SystemController. The UndoRedoSystem manage all commands and queries asynchronously in an ordered manner. 

### Desktop

Only UI logic. All calls are to controllers are asynchornous and replies are received using signals.

# Build and Test

Qt Creator with Qt6