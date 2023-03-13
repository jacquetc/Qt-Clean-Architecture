#pragma once

#include "contracts_global.h"
#include <QUuid>

namespace Contracts::CQRS::Author::Commands
{
class SKRCONTRACTSEXPORT RemoveAuthorCommand
{
  public:
    RemoveAuthorCommand()
    {
    }

    QUuid id;
};
} // namespace Contracts::CQRS::Author::Commands
