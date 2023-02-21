#pragma once

#include "contracts_global.h"
#include <QUuid>

namespace Contracts::CQRS::Author::Requests
{
class SKRCONTRACTSEXPORT GetAuthorRequest
{
  public:
    GetAuthorRequest()
    {
    }

    QUuid id;
};
} // namespace Contracts::CQRS::Author::Requests
