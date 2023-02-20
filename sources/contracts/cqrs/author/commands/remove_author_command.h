#pragma once

#include <QUuid>

namespace Contracts::CQRS::Author::Commands
{
class RemoveAuthorCommand
{
  public:
    RemoveAuthorCommand()
    {
    }

    QUuid id;
};
} // namespace Contracts::CQRS::Author::Commands
