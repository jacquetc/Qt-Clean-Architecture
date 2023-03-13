#include "persistence/interface_repository.h"
#include <QSharedPointer>

#pragma once
namespace Contracts::Persistence

{
class InterfaceRepositoryProvider
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
