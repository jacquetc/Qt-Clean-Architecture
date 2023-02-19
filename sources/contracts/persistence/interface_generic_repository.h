#ifndef INTERFACE_GENERIC_REPOSITORY_H
#define INTERFACE_GENERIC_REPOSITORY_H

#include <QFuture>
#include <QUuid>

#include "contracts_global.h"
#include "database/interface_database.h"
#include "interface_repository.h"
#include "result.h"

namespace Contracts::Persistence
{
template <class T> class SKRCONTRACTSEXPORT InterfaceGenericRepository : public InterfaceRepository
{
  public:
    virtual ~InterfaceGenericRepository()
    {
    }

    // classes can clean up

    virtual Result<T> get(const QUuid &uuid) = 0;

    virtual Result<QList<T>> getAll() = 0;

    virtual Result<T> remove(T &&entity) = 0;
    virtual Result<T> add(T &&entity) = 0;
    virtual Result<T> update(T &&entity) = 0;
    virtual Result<bool> exists(const QUuid &uuid) = 0;
    virtual void setDatabase(Contracts::Database::InterfaceDatabase<T> *database) = 0;
    virtual Contracts::Database::InterfaceDatabase<T> *database() = 0;
};
} // namespace Contracts::Persistence

#endif // INTERFACE_GENERIC_REPOSITORY_H
