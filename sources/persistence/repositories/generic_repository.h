#ifndef GENERIC_REPOSITORY_H
#define GENERIC_REPOSITORY_H

#include "QtConcurrent/qtconcurrenttask.h"
#include "database/interface_database.h"
#include "persistence/interface_generic_repository.h"
#include "persistence_global.h"
#include "result.h"

#include <QEventLoop>
#include <QFuture>
#include <QFutureWatcher>
#include <QObject>
#include <QReadWriteLock>
#include <QUuid>

using namespace Contracts::Database;

namespace Repository
{
// -------------------------------------------------

template <class T>
class SKRPERSISTENCEEXPORT GenericRepository : public virtual Contracts::Persistence::InterfaceGenericRepository<T>
{

  public:
    // InterfaceGenericRepository interface

  public:
    GenericRepository()
    {
    }

    Result<T> get(const QUuid &uuid) override;

    Result<QList<T>> getAll() override;

    Result<T> remove(T &&entity) override;

    Result<T> add(T &&entity) override;

    Result<T> update(T &&entity) override;

    Result<bool> exists(const QUuid &uuid) override;

  protected:
    template <class U> auto waitInEventLoop(QFuture<Result<U>> &&future)
    {
        QEventLoop wait;
        QFutureWatcher<Result<U>> fw;

        QObject::connect(&fw, &QFutureWatcher<Result<U>>::finished, &wait, &QEventLoop::quit);

        // Connect progressRangeChanged signal to a lambda function that emits a signal with progress info
        QObject::connect(&fw, &QFutureWatcher<Result<U>>::progressRangeChanged, [this](int minimum, int maximum) {
            // emit a signal with progress info
            setProgressChanged(minimum, maximum, 0);
        });

        // Connect progressValueChanged signal to a lambda function that emits a signal with progress info
        QObject::connect(&fw, &QFutureWatcher<Result<U>>::progressValueChanged, [this](int progressValue) {
            // emit a signal with progress info
            setProgressChanged(0, 0, progressValue);
        });

        fw.setFuture(future);
        wait.exec();

        // Disconnect the signals after the event loop has finished
        QObject::disconnect(&fw, &QFutureWatcher<Result<U>>::finished, &wait, &QEventLoop::quit);
        QObject::disconnect(&fw, &QFutureWatcher<Result<U>>::progressRangeChanged, nullptr, nullptr);
        QObject::disconnect(&fw, &QFutureWatcher<Result<U>>::progressValueChanged, nullptr, nullptr);

        return fw.result();
    }

    virtual void setProgressChanged(int minimum, int maximum, int value) = 0;

  private:
    mutable QReadWriteLock m_lock;
    InterfaceDatabase<T> *m_database;

    // InterfaceGenericRepository interface
  public:
    void setDatabase(InterfaceDatabase<T> *database) override;
    InterfaceDatabase<T> *database() override;

    // InterfaceRepository interface
  public:
    void setDatabaseContext(InterfaceDatabaseContext *databaseContext) override;
    InterfaceDatabaseContext *databaseContext() override;
};

template <class T> void GenericRepository<T>::setDatabaseContext(InterfaceDatabaseContext *databaseContext)
{
    m_database->setDatabaseContext(databaseContext);
}

template <class T> InterfaceDatabaseContext *GenericRepository<T>::databaseContext()
{
    m_database->databaseContext();
}

template <class T> void GenericRepository<T>::setDatabase(InterfaceDatabase<T> *database)
{
    m_database = database;
}

template <class T> InterfaceDatabase<T> *GenericRepository<T>::database()
{
    return m_database;
}

template <class T> Result<T> GenericRepository<T>::get(const QUuid &uuid)
{
    QReadLocker locker(&m_lock);
    return waitInEventLoop(
        QtConcurrent::task([](InterfaceDatabase<T> *database, QUuid uuid) { return database->get(uuid); })
            .withArguments(m_database, uuid)
            .spawn());
}

template <class T> Result<QList<T>> GenericRepository<T>::getAll()
{
    QReadLocker locker(&m_lock);
    return waitInEventLoop(QtConcurrent::task([](InterfaceDatabase<T> *database) { return database->getAll(); })
                               .withArguments(m_database)
                               .spawn());
}

template <class T> Result<T> GenericRepository<T>::remove(T &&entity)
{
    QWriteLocker locker(&m_lock);
    return waitInEventLoop(
        QtConcurrent::task([](InterfaceDatabase<T> *database, T entity) { return database->remove(std::move(entity)); })
            .withArguments(m_database, std::move(entity))
            .spawn());
}

template <class T> Result<T> GenericRepository<T>::add(T &&entity)
{
    QWriteLocker locker(&m_lock);
    return waitInEventLoop(
        QtConcurrent::task([](InterfaceDatabase<T> *database, T entity) { return database->add(std::move(entity)); })
            .withArguments(m_database, std::move(entity))
            .spawn());
}

template <class T> Result<T> GenericRepository<T>::update(T &&entity)
{
    QWriteLocker locker(&m_lock);
    return waitInEventLoop(
        QtConcurrent::task([](InterfaceDatabase<T> *database, T entity) { return database->update(std::move(entity)); })
            .withArguments(m_database, std::move(entity))
            .spawn());
}

template <class T> Result<bool> GenericRepository<T>::exists(const QUuid &uuid)
{
    QReadLocker locker(&m_lock);

    return waitInEventLoop(
        QtConcurrent::task([](InterfaceDatabase<T> *database, QUuid uuid) { return database->exists(uuid); })
            .withArguments(m_database, uuid)
            .spawn());
}

} // namespace Repository

#endif // GENERIC_REPOSITORY_H
