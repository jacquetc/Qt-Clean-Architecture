#include "get_author_request_handler.h"
#include "automapper/automapper.h"
#include "persistence/interface_author_repository.h"

using namespace Application::Features::Author::Queries;

GetAuthorRequestHandler::GetAuthorRequestHandler(InterfaceRepositories *repositories)
    : Handler(), m_repositories(repositories)
{
}

Result<AuthorDTO> GetAuthorRequestHandler::handle(const GetAuthorRequest &request)
{
    // get repository
    InterfaceAuthorRepository *repository =
        dynamic_cast<InterfaceAuthorRepository *>(m_repositories->get(InterfaceRepositories::Entities::Author));

    // do
    auto authorResult = repository->get(request.id);

    if (authorResult.isError())
    {
        return Result<AuthorDTO>(authorResult.error());
    }

    // map
    auto dto = AutoMapper::AutoMapper::map<AuthorDTO>(authorResult.value());
    return Result<AuthorDTO>(dto);
}
