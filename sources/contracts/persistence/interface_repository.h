#pragma once
#include "contracts_global.h"
#include "database/interface_database_context.h"

class SKRCONTRACTSEXPORT InterfaceRepository
{

  public:
    virtual ~InterfaceRepository()
    {
    }
    virtual void setDatabaseContext(Contracts::Database::InterfaceDatabaseContext *databaseContext) = 0;
    virtual Contracts::Database::InterfaceDatabaseContext *databaseContext() = 0;
};
