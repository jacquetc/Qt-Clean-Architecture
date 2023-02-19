#include "remove_author_command_handler.h"
#include "automapper/automapper.h"

using namespace Contracts::DTO::Author;
using namespace Contracts::Persistence;
using namespace Contracts::CQRS::Author::Commands;
using namespace Application::Features::Author::Commands;

RemoveAuthorCommandHandler::RemoveAuthorCommandHandler(InterfaceAuthorRepository *repository) : m_repository(repository)
{
}

Result<AuthorDTO> RemoveAuthorCommandHandler::handle(const RemoveAuthorCommand &request)
{

    Result<Domain::Author> authorResult = m_repository->get(request.id);
    if (authorResult.hasError())
    {
        return Result<AuthorDTO>(authorResult.error());
    }
    auto deleteResult = m_repository->add(std::move(authorResult.value()));
    if (deleteResult.hasError())
    {
        return Result<AuthorDTO>(deleteResult.error());
    }
    // map
    auto dto = AutoMapper::AutoMapper::map<AuthorDTO>(deleteResult.value());
    return Result<AuthorDTO>(dto);
}
