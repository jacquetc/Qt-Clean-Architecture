# Introduction 

Example Qt 6 project structure demonstrating the implementation of Clean Architecture principles for maintainable and scalable software development. 

The basic concepts come from the book *Clean Architecture: A Craftsman's Guide to Software Structure and Design* by Robert C. Martin and the teachings of Bernardo Estacio Abreu, Software Architect. This proof of concept was initiated so as to offer a new base for the software Skribisto.

This example is minimal but functional and tested. This is a basis for a desktop (QtWidget) application.


Main points:
- one simple entity: Author
- a very basic QWidget UI 
- asynchronous API
- asynchronous undo redo system
- automapper
- Command and Query Responsibility Segregation ([CQRS](https://learn.microsoft.com/en-us/azure/architecture/patterns/cqrs))
- use of Data Transfert Objects ([DTO](https://www.baeldung.com/java-dto-pattern))
- file load and save
- error handling

Tested successfuly on Qt 6.2.4 and 6.5-beta3.

# Structure

![Clean Architecture diagram](/resources/docs/qt-clean-arch.png)

## Core

Business side of the clean architecture.

### Domain

Contains the entities. Simple inert QObjects exposing their fields using Q_PROPERTY

### Persistence

#### Database

Internal database. Here a simple SQLite. They could be a bunch of QList or an online database if needed.

#### Repositories

Agnostic wrapper around the database. One repository per entity.
### Contracts

Used to enforce the [Interface Segregation Principle](https://stackify.com/interface-segregation-principle/) and the [ependency Inversion Principle](https://stackify.com/dependency-inversion-principle/). Also contains DTOs and CQRS implementation which must be usable to all parts. All parts, but domains, depend of this library.

### Infrastructure

Here lie the connections of to the external services, like loading and saving files.

### Application

Use cases. All the business logics.

## GUI

Front side of the clean architecture.

### Presenter

Each feature offers its own Controller singleton such as AuthorController or SystemController. It gives a stable and clear API for the UI. The UndoRedoSystem manage all commands and queries asynchronously in an ordered manner. 

### Desktop

Only UI logic. All calls are to controllers are asynchornous and replies are received using signals.

# Build and Test

Qt Creator with minimum Qt 6.2

# QML

 Minimal adaptations are needed for a QML application : 
 - allow the controllers (in the presenter) to appear in QML
 - add a new part in the GUI folder (ex: sribisto-mobile) with a main.cpp inside. It will contain all the QML files.

Of course, you can make a desktop UI and QML UI to coexist, only in separate folders.

# Qt Models

QAbstractItemModels (and the others inheriting it) would be in the UI side (Desktop) and use the presenter's controllers for all operations 
with the backend.