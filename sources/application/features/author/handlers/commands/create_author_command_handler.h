#ifndef CREATE_AUTHOR_COMMAND_HANDLER_H
#define CREATE_AUTHOR_COMMAND_HANDLER_H

#include "cqrs/author/commands/create_author_command.h"
#include "persistence/interface_repositories.h"
#include "result.h"

using namespace Contracts::DTO::Author;
using namespace Contracts::Persistence;
using namespace Contracts::CQRS::Author::Commands;

namespace Application::Features::Author::Commands
{
class CreateAuthorCommandHandler
{
  public:
    CreateAuthorCommandHandler(InterfaceRepositories *repositories);
    Result<QUuid> handle(const CreateAuthorCommand &request);

  private:
    InterfaceRepositories *m_repositories;
};
} // namespace Application::Features::Author::Commands

#endif // CREATE_AUTHOR_COMMAND_HANDLER_H
