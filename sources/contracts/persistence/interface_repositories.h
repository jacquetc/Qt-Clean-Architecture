#pragma once

#include "database/interface_database_context.h"
#include "persistence/interface_repository.h"

using namespace Contracts::Database;

namespace Contracts::Persistence
{
class InterfaceRepositories
{

  public:
    enum Entities
    {
        Author
    };

    virtual ~InterfaceRepositories()
    {
    }

    virtual void append(Entities entity, InterfaceRepository *repository) = 0;
    virtual InterfaceRepository *get(Entities entity) = 0;
    virtual void setDatabaseContext(InterfaceDatabaseContext *context) = 0;
};
} // namespace Contracts::Persistence
