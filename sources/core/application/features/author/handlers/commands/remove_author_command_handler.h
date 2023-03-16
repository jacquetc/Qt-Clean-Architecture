#pragma once

#include "application_global.h"
#include "cqrs/author/commands/remove_author_command.h"
#include "dto/author/author_dto.h"
#include "handler.h"
#include "persistence/interface_author_repository.h"
#include "result.h"

using namespace Contracts::DTO::Author;
using namespace Contracts::Persistence;
using namespace Contracts::CQRS::Author::Commands;

namespace Application::Features::Author::Commands
{
class SKR_APPLICATION_EXPORT RemoveAuthorCommandHandler : public Handler
{
    Q_OBJECT
  public:
    RemoveAuthorCommandHandler(QSharedPointer<InterfaceAuthorRepository> repository);
    Result<AuthorDTO> handle(const RemoveAuthorCommand &request);

  private:
    QSharedPointer<InterfaceAuthorRepository> m_repository;
    Result<AuthorDTO> handleImpl(const RemoveAuthorCommand &request);
};
} // namespace Application::Features::Author::Commands
