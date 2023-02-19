#ifndef GET_AUTHOR_REQUEST_HANDLER_H
#define GET_AUTHOR_REQUEST_HANDLER_H

#include "persistence/interface_author_repository.h"

#include "cqrs/author/requests/get_author_with_details_request.h"

#include "dto/author/author_dto.h"

using namespace Contracts::DTO::Author;
using namespace Contracts::Persistence;
using namespace Contracts::CQRS::Author::Requests;

namespace Application::Features::Author::Queries
{
class GetAuthorRequestHandler
{
  public:
    GetAuthorRequestHandler(InterfaceAuthorRepository *repository);
    Result<AuthorDTO> handle(const GetAuthorRequest &request);

  private:
    InterfaceAuthorRepository *m_repository;
};
} // namespace Application::Features::Author::Queries

#endif // GET_AUTHOR_REQUEST_HANDLER_H
