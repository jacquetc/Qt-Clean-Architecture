#pragma once

#include <QUuid>

namespace Contracts::CQRS::Author::Requests
{
class GetAuthorRequest
{
  public:
    GetAuthorRequest()
    {
    }

    QUuid id;
};
} // namespace Contracts::CQRS::Author::Requests
