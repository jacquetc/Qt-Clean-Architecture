#pragma once

#include "application_global.h"
#include "cqrs/author/commands/update_author_command.h"
#include "dto/author/author_dto.h"
#include "handler.h"
#include "persistence/interface_author_repository.h"
#include "result.h"

using namespace Contracts::DTO::Author;
using namespace Contracts::Persistence;
using namespace Contracts::CQRS::Author::Commands;

namespace Application::Features::Author::Commands
{
class SKR_APPLICATION_EXPORT UpdateAuthorCommandHandler : public Handler

{
    Q_OBJECT
  public:
    UpdateAuthorCommandHandler(QSharedPointer<InterfaceAuthorRepository> repository);
    Result<AuthorDTO> handle(const UpdateAuthorCommand &request);

  private:
    QSharedPointer<InterfaceAuthorRepository> m_repository;
    Result<AuthorDTO> handleImpl(const UpdateAuthorCommand &request);
};
} // namespace Application::Features::Author::Commands
