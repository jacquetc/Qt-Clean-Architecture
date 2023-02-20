#pragma once

#include "cqrs/author/commands/update_author_command.h"
#include "dto/author/author_dto.h"
#include "handler.h"
#include "persistence/interface_repositories.h"
#include "result.h"

using namespace Contracts::DTO::Author;
using namespace Contracts::Persistence;
using namespace Contracts::CQRS::Author::Commands;

namespace Application::Features::Author::Commands
{
class UpdateAuthorCommandHandler : public Handler

{
  public:
    UpdateAuthorCommandHandler(InterfaceRepositories *repositories);
    Result<AuthorDTO> handle(const UpdateAuthorCommand &request);

  private:
    InterfaceRepositories *m_repositories;
};
} // namespace Application::Features::Author::Commands
