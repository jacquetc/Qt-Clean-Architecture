#ifndef INTERFACE_AUTHOR_REPOSITORY_H
#define INTERFACE_AUTHOR_REPOSITORY_H

#include "author.h"
#include "contracts_global.h"
#include "interface_generic_repository.h"

#include <QFuture>
#include <QSharedPointer>

namespace Contracts::Persistence
{
class SKRCONTRACTSEXPORT InterfaceAuthorRepository
    : public virtual Contracts::Persistence::InterfaceGenericRepository<Domain::Author>
{
  public:
    virtual ~InterfaceAuthorRepository()
    {
    }
};
} // namespace Contracts::Persistence

#endif // INTERFACE_AUTHOR_REPOSITORY_H
