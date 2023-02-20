#include "remove_author_command_handler.h"
#include "automapper/automapper.h"
#include "persistence/interface_author_repository.h"

using namespace Contracts::DTO::Author;
using namespace Contracts::Persistence;
using namespace Contracts::CQRS::Author::Commands;
using namespace Application::Features::Author::Commands;

RemoveAuthorCommandHandler::RemoveAuthorCommandHandler(InterfaceRepositories *repositories)
    : Handler(), m_repositories(repositories)
{
}

Result<AuthorDTO> RemoveAuthorCommandHandler::handle(const RemoveAuthorCommand &request)
{
    // get repository
    InterfaceAuthorRepository *repository =
        dynamic_cast<InterfaceAuthorRepository *>(m_repositories->get(InterfaceRepositories::Entities::Author));

    Result<Domain::Author> authorResult = repository->get(request.id);
    if (authorResult.hasError())
    {
        return Result<AuthorDTO>(authorResult.error());
    }
    auto deleteResult = repository->add(std::move(authorResult.value()));
    if (deleteResult.hasError())
    {
        return Result<AuthorDTO>(deleteResult.error());
    }
    // map
    auto dto = AutoMapper::AutoMapper::map<AuthorDTO>(deleteResult.value());
    return Result<AuthorDTO>(dto);
}
