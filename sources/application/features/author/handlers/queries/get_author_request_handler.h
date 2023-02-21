#pragma once

#include "application_global.h"
#include "cqrs/author/requests/get_author_request.h"
#include "dto/author/author_dto.h"
#include "handler.h"
#include "persistence/interface_repositories.h"

using namespace Contracts::DTO::Author;
using namespace Contracts::Persistence;
using namespace Contracts::CQRS::Author::Requests;

namespace Application::Features::Author::Queries
{
class SKRAPPLICATIONEXPORT GetAuthorRequestHandler : public Handler
{
  public:
    GetAuthorRequestHandler(InterfaceRepositories *repositories);
    Result<AuthorDTO> handle(const GetAuthorRequest &request);

  private:
    InterfaceRepositories *m_repositories;
};
} // namespace Application::Features::Author::Queries
