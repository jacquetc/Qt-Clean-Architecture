#ifndef REMOVE_AUTHOR_COMMAND_HANDLER_H
#define REMOVE_AUTHOR_COMMAND_HANDLER_H

#include "cqrs/author/commands/remove_author_command.h"
#include "dto/author/author_dto.h"
#include "handler.h"
#include "persistence/interface_repositories.h"
#include "result.h"

using namespace Contracts::DTO::Author;
using namespace Contracts::Persistence;
using namespace Contracts::CQRS::Author::Commands;

namespace Application::Features::Author::Commands
{
class RemoveAuthorCommandHandler : public Handler
{
  public:
    RemoveAuthorCommandHandler(InterfaceRepositories *repositories);
    Result<AuthorDTO> handle(const RemoveAuthorCommand &request);

  private:
    InterfaceRepositories *m_repositories;
};
} // namespace Application::Features::Author::Commands

#endif // REMOVEAUTHORCOMMANDHANDLER_H
