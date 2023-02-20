#include "create_author_command_handler.h"
#include "automapper/automapper.h"
#include "cqrs/author/validators/create_author_command_validator.h"
#include "persistence/interface_author_repository.h"

using namespace Contracts::DTO::Author;
using namespace Contracts::Persistence;
using namespace Contracts::CQRS::Author::Validators;
using namespace Application::Features::Author::Commands;

CreateAuthorCommandHandler::CreateAuthorCommandHandler(InterfaceRepositories *repositories)
    : Handler(), m_repositories(repositories)
{
}

Result<QUuid> CreateAuthorCommandHandler::handle(const CreateAuthorCommand &request)
{
    // get repository
    InterfaceAuthorRepository *repository =
        dynamic_cast<InterfaceAuthorRepository *>(m_repositories->get(InterfaceRepositories::Entities::Author));

    // validate:
    auto validator = CreateAuthorCommandValidator(repository);
    Result<void *> validatorResult = validator.validate(request.req);
    if (validatorResult.hasError())
    {
        return Result<QUuid>(validatorResult.error());
    }

    auto author = AutoMapper::AutoMapper::map<Domain::Author>(request.req);
    author.setUuid(QUuid::createUuid());

    // set timestamp
    author.setCreationDate(QDateTime::currentDateTime());
    author.setUpdateDate(QDateTime::currentDateTime());

    // do
    auto authorResult = repository->add(std::move(author));
    if (authorResult.hasError())
    {
        return Result<QUuid>(authorResult.error());
    }

    return Result<QUuid>(authorResult.value().uuid());
}
