#pragma once
#include "contracts_global.h"
#include <QThreadPool>

namespace Contracts::Database
{
class SKRCONTRACTSEXPORT InterfaceDatabaseContext
{
  public:
    virtual ~InterfaceDatabaseContext()
    {
    }

    virtual QString databaseName() const = 0;
    virtual QThreadPool &threadPool() = 0;
};
} // namespace Contracts::Database
