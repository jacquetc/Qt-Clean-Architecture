#ifndef GET_AUTHOR_REQUEST_HANDLER_H
#define GET_AUTHOR_REQUEST_HANDLER_H

#include "handler.h"
#include "persistence/interface_repositories.h"

#include "cqrs/author/requests/get_author_with_details_request.h"

#include "dto/author/author_dto.h"

using namespace Contracts::DTO::Author;
using namespace Contracts::Persistence;
using namespace Contracts::CQRS::Author::Requests;

namespace Application::Features::Author::Queries
{
class GetAuthorRequestHandler : public Handler
{
  public:
    GetAuthorRequestHandler(InterfaceRepositories *repositories);
    Result<AuthorDTO> handle(const GetAuthorRequest &request);

  private:
    InterfaceRepositories *m_repositories;
};
} // namespace Application::Features::Author::Queries

#endif // GET_AUTHOR_REQUEST_HANDLER_H
