#include "update_author_command_handler.h"
#include "automapper/automapper.h"
#include "cqrs/author/validators/update_author_command_validator.h"
#include "persistence/interface_author_repository.h"

using namespace Contracts::DTO::Author;
using namespace Contracts::Persistence;
using namespace Contracts::CQRS::Author::Commands;
using namespace Contracts::CQRS::Author::Validators;
using namespace Application::Features::Author::Commands;

UpdateAuthorCommandHandler::UpdateAuthorCommandHandler(InterfaceRepositories *repositories)
    : Handler(), m_repositories(repositories)
{
}

Result<AuthorDTO> Application::Features::Author::Commands::UpdateAuthorCommandHandler::handle(
    const UpdateAuthorCommand &request)
{
    InterfaceAuthorRepository *repository =
        dynamic_cast<InterfaceAuthorRepository *>(m_repositories->get(InterfaceRepositories::Entities::Author));

    // validate:
    auto validator = UpdateAuthorCommandValidator(repository);
    Result<void *> validatorResult = validator.validate(request.req);
    if (validatorResult.hasError())
    {
        return Result<AuthorDTO>(validatorResult.error());
    }

    // map
    auto author = AutoMapper::AutoMapper::map<Domain::Author>(request.req);

    // set update timestamp
    author.setUpdateDate(QDateTime::currentDateTime());
    // do
    auto authorResult = repository->update(std::move(author));
    if (authorResult.hasError())
    {
        return Result<AuthorDTO>(authorResult.error());
    }
    // map
    auto authorDto = AutoMapper::AutoMapper::map<AuthorDTO>(authorResult.value());

    return Result<AuthorDTO>(authorDto);
}
