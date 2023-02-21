#pragma once

#include "application_global.h"
#include "dto/author/author_dto.h"
#include "handler.h"
#include "persistence/interface_repositories.h"

using namespace Contracts::DTO::Author;
using namespace Contracts::Persistence;

namespace Application::Features::Author::Queries
{
class SKRAPPLICATIONEXPORT GetAuthorListRequestHandler : public Handler
{
  public:
    GetAuthorListRequestHandler(InterfaceRepositories *repositories);
    Result<QList<AuthorDTO>> handle();

  private:
    InterfaceRepositories *m_repositories;
};
} // namespace Application::Features::Author::Queries
