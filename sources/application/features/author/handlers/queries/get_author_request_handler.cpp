#include "get_author_request_handler.h"
#include "automapper/automapper.h"

using namespace Application::Features::Author::Queries;

GetAuthorRequestHandler::GetAuthorRequestHandler(InterfaceAuthorRepository *repository)
    : m_repository(repository)
{
}

Result<AuthorDTO> GetAuthorRequestHandler::handle(const GetAuthorRequest &request)
{
    auto authorResult = m_repository->get(request.id);

    if (authorResult.isError())
    {
        return Result<AuthorDTO>(authorResult.error());
    }

    // do
    auto dto = AutoMapper::AutoMapper::map<AuthorDTO>(authorResult.value());
    return Result<AuthorDTO>(dto);
}
