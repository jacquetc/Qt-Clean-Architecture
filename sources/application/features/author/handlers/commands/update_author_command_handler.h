#ifndef UPDATEAUTHORCOMMANDHANDLER_H
#define UPDATEAUTHORCOMMANDHANDLER_H

#include "cqrs/author/commands/update_author_command.h"
#include "dto/author/author_dto.h"
#include "persistence/interface_author_repository.h"

using namespace Contracts::DTO::Author;
using namespace Contracts::Persistence;
using namespace Contracts::CQRS::Author::Commands;

namespace Application::Features::Author::Commands
{
class UpdateAuthorCommandHandler
{
  public:
    UpdateAuthorCommandHandler(InterfaceAuthorRepository *repository);
    Result<AuthorDTO> handle(const UpdateAuthorCommand &request);

  private:
    InterfaceAuthorRepository *m_repository;
};
} // namespace Application::Features::Author::Commands

#endif // UPDATEAUTHORCOMMANDHANDLER_H
