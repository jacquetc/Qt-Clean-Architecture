#ifndef INTERFACE_DATABASE_CONTEXT_H
#define INTERFACE_DATABASE_CONTEXT_H
#include "contracts_global.h"

namespace Contracts::Database
{
class SKRCONTRACTSEXPORT InterfaceDatabaseContext
{
  public:
    virtual ~InterfaceDatabaseContext()
    {
    }
};
} // namespace Contracts::Database

#endif // INTERFACE_DATABASE_CONTEXT_H
