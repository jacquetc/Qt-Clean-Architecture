#include "persistence/interface_repository.h"
#include "contracts_global.h"
#include <QSharedPointer>

#pragma once
namespace Contracts::Persistence

{
class SKR_CONTRACTS_EXPORT InterfaceRepositoryProvider
{

  public:
    enum Entities
    {
        Author
    };

    virtual ~InterfaceRepositoryProvider()
    {
    }

    virtual void registerRepository(Entities entity, QSharedPointer<InterfaceRepository> repository) = 0;
    virtual QSharedPointer<InterfaceRepository> repository(Entities entity) = 0;
};
} // namespace Contracts::Persistence
