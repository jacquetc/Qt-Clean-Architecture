#ifndef INTERFACE_REPOSITORY_H
#define INTERFACE_REPOSITORY_H
#include "database/interface_database_context.h"

class InterfaceRepository {

public:
    virtual ~InterfaceRepository(){}
    virtual void setDatabaseContext(Contracts::Database::InterfaceDatabaseContext *databaseContext) = 0;
    virtual Contracts::Database::InterfaceDatabaseContext *databaseContext() = 0;
};

#endif // INTERFACE_REPOSITORY_H
