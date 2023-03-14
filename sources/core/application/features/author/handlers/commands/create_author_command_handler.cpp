#include "create_author_command_handler.h"
#include "automapper/automapper.h"
#include "cqrs/author/validators/create_author_command_validator.h"
#include "persistence/interface_author_repository.h"

using namespace Contracts::DTO::Author;
using namespace Contracts::Persistence;
using namespace Contracts::CQRS::Author::Validators;
using namespace Application::Features::Author::Commands;

CreateAuthorCommandHandler::CreateAuthorCommandHandler(QSharedPointer<InterfaceAuthorRepository> repository)
    : Handler(), m_repository(repository)
{
}
Result<QUuid> CreateAuthorCommandHandler::handle(const CreateAuthorCommand &request)
{
    Result<QUuid> result;

    try
    {
        result = handleImpl(request);
    }
    catch (const std::exception &ex)
    {
        result = Result<QUuid>(Error(this, Error::Critical, "Unknown error", ex.what()));
        qDebug() << "Error handling CreateAuthorCommand:" << ex.what();
    }

    return result;
}

Result<QUuid> CreateAuthorCommandHandler::handleImpl(const CreateAuthorCommand &request)
{
    qDebug() << "CreateAuthorCommandHandler::handleImpl called";

    // Validate the create author command using the validator
    auto validator = CreateAuthorCommandValidator(m_repository);
    Result<void *> validatorResult = validator.validate(request.req);
    if (validatorResult.hasError())
    {
        return Result<QUuid>(validatorResult.error());
    }

    // Map the create author command to a domain author object and generate a UUID
    auto author = AutoMapper::AutoMapper::map<Domain::Author>(request.req);

    // allow for forcing the uuid
    if (author.uuid().isNull())
    {
        author.setUuid(QUuid::createUuid());
    }

    // Set the creation and update timestamps to the current date and time
    author.setCreationDate(QDateTime::currentDateTime());
    author.setUpdateDate(QDateTime::currentDateTime());

    // Add the author to the repository
    auto authorResult = m_repository->add(std::move(author));
    if (authorResult.hasError())
    {
        return Result<QUuid>(authorResult.error());
    }
    qDebug() << "Author added:" << authorResult.value().uuid();

    // Return the UUID of the newly created author as a Result object
    return Result<QUuid>(authorResult.value().uuid());
}
