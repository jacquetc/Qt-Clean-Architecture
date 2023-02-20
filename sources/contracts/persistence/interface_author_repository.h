#pragma once

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
