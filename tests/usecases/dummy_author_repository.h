#ifndef DUMMY_AUTHOR_REPOSITORY_H
#define DUMMY_AUTHOR_REPOSITORY_H

#include "author.h"
#include "database/interface_database.h"
#include "persistence/interface_author_repository.h"

#include <QObject>

class DummyAuthorRepository : public QObject, public Contracts::Persistence::InterfaceAuthorRepository
{
    Q_OBJECT

  public:
    DummyAuthorRepository(QObject *parent) : QObject{parent}, m_exists(true)
    {
    }

    void fillGet(const Domain::Author &entity);
    void fillGetAll(const QList<Domain::Author> &list);
    void fillRemove(const Domain::Author &entity);
    void fillAdd(const Domain::Author &entity);
    void fillUpdate(const Domain::Author &entity);
    void fillExists(bool value);

    // InterfaceGenericRepository interface

  public:
    Result<Domain::Author> get(const QUuid &uuid) override;
    Result<QList<Domain::Author>> getAll() override;
    Result<Domain::Author> remove(Domain::Author &&entity) override;
    Result<Domain::Author> add(Domain::Author &&entity) override;
    Result<Domain::Author> update(Domain::Author &&entity) override;
    Result<bool> exists(const QUuid &uuid) override;

  private:
    Domain::Author m_getEntity;
    QList<Domain::Author> m_getAllList;
    Domain::Author m_removeEntity;
    Domain::Author m_addEntity;
    Domain::Author m_updateEntity;
    bool m_exists;

    // InterfaceGenericRepository interface
  public:
    void setDatabase(Contracts::Database::InterfaceDatabase<Domain::Author> *database) override;
    Contracts::Database::InterfaceDatabase<Domain::Author> *database() override;

    // InterfaceRepository interface
  public:
    void setDatabaseContext(Contracts::Database::InterfaceDatabaseContext *databaseContext) override;
    Contracts::Database::InterfaceDatabaseContext *databaseContext() override;
};

inline void DummyAuthorRepository::setDatabaseContext(Contracts::Database::InterfaceDatabaseContext *databaseContext)
{
}

inline Contracts::Database::InterfaceDatabaseContext *DummyAuthorRepository::databaseContext()
{
    return nullptr;
}

inline void DummyAuthorRepository::setDatabase(Contracts::Database::InterfaceDatabase<Domain::Author> *database)
{
}

inline Contracts::Database::InterfaceDatabase<Domain::Author> *DummyAuthorRepository::database()
{
    return nullptr;
}

inline void DummyAuthorRepository::fillGet(const Domain::Author &entity)
{
    m_getEntity = entity;
}

inline void DummyAuthorRepository::fillGetAll(const QList<Domain::Author> &list)
{
    m_getAllList = list;
}

inline void DummyAuthorRepository::fillRemove(const Domain::Author &entity)
{
    m_removeEntity = entity;
}

inline void DummyAuthorRepository::fillAdd(const Domain::Author &entity)
{
    m_addEntity = entity;
}

inline void DummyAuthorRepository::fillUpdate(const Domain::Author &entity)
{
    m_updateEntity = entity;
}

inline void DummyAuthorRepository::fillExists(bool value)
{
    m_exists = value;
}

inline Result<Domain::Author> DummyAuthorRepository::get(const QUuid &uuid)
{
    return Result<Domain::Author>(m_getEntity);
}

inline Result<QList<Domain::Author>> DummyAuthorRepository::getAll()
{
    return Result<QList<Domain::Author>>(m_getAllList);
}

inline Result<Domain::Author> DummyAuthorRepository::remove(Domain::Author &&entity)
{
    return Result<Domain::Author>(m_removeEntity);
}

inline Result<Domain::Author> DummyAuthorRepository::add(Domain::Author &&entity)
{
    return Result<Domain::Author>(m_addEntity);
}

inline Result<Domain::Author> DummyAuthorRepository::update(Domain::Author &&entity)
{
    return Result<Domain::Author>(m_updateEntity);
}

inline Result<bool> DummyAuthorRepository::exists(const QUuid &uuid)
{
    return Result<bool>(m_exists);
}

#endif // DUMMY_AUTHOR_REPOSITORY_H
