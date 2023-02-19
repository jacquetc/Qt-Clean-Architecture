#ifndef REMOVE_AUTHOR_COMMAND_HANDLER_H
#define REMOVE_AUTHOR_COMMAND_HANDLER_H

#include "cqrs/author/commands/remove_author_command.h"
#include "dto/author/author_dto.h"
#include "persistence/interface_author_repository.h"

using namespace Contracts::DTO::Author;
using namespace Contracts::Persistence;
using namespace Contracts::CQRS::Author::Commands;

namespace Application::Features::Author::Commands
{
class RemoveAuthorCommandHandler
{
  public:
    RemoveAuthorCommandHandler(InterfaceAuthorRepository *repository);
    Result<AuthorDTO> handle(const RemoveAuthorCommand &request);

  private:
    InterfaceAuthorRepository *m_repository;
};
} // namespace Application::Features::Author::Commands

#endif // REMOVEAUTHORCOMMANDHANDLER_H
