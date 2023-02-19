#include "update_author_command_handler.h"
#include "automapper/automapper.h"
#include "cqrs/author/validators/update_author_command_validator.h"

using namespace Contracts::DTO::Author;
using namespace Contracts::Persistence;
using namespace Contracts::CQRS::Author::Commands;
using namespace Contracts::CQRS::Author::Validators;
using namespace Application::Features::Author::Commands;

UpdateAuthorCommandHandler::UpdateAuthorCommandHandler(InterfaceAuthorRepository *repository) : m_repository(repository)
{
}

Result<AuthorDTO> Application::Features::Author::Commands::UpdateAuthorCommandHandler::handle(
    const UpdateAuthorCommand &request)
{
    // validate:
    auto validator = UpdateAuthorCommandValidator(m_repository);
    Result<void *> validatorResult = validator.validate(request.req);
    if (validatorResult.hasError())
    {
        return Result<AuthorDTO>(validatorResult.error());
    }

    // map
    auto author = AutoMapper::AutoMapper::map<Domain::Author>(request.req);

    // do
    auto authorResult = m_repository->update(std::move(author));
    if (authorResult.hasError())
    {
        return Result<AuthorDTO>(authorResult.error());
    }
    // map
    auto authorDto = AutoMapper::AutoMapper::map<AuthorDTO>(authorResult.value());

    return Result<AuthorDTO>(authorDto);
}
