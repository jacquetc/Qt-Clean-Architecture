#include "update_author_command_handler.h"
#include "QtConcurrent/qtconcurrentrun.h"
#include "automapper/automapper.h"
#include "cqrs/author/validators/update_author_command_validator.h"
#include "persistence/interface_author_repository.h"

using namespace Contracts::DTO::Author;
using namespace Contracts::Persistence;
using namespace Contracts::CQRS::Author::Commands;
using namespace Contracts::CQRS::Author::Validators;
using namespace Application::Features::Author::Commands;

UpdateAuthorCommandHandler::UpdateAuthorCommandHandler(QSharedPointer<InterfaceAuthorRepository> repository)
    : Handler(), m_repository(repository)
{
}
Result<AuthorDTO> UpdateAuthorCommandHandler::handle(const UpdateAuthorCommand &request)
{
    Result<AuthorDTO> result;

    try
    {
        result = handleImpl(request);
        emit authorUpdated(result);
    }
    catch (const std::exception &ex)
    {
        result = Result<AuthorDTO>(Error(this, Error::Critical, "Unknown error", ex.what()));
        qDebug() << "Error handling UpdateAuthorCommand:" << ex.what();
        emit authorUpdated(result);
    }

    return result;
}

void UpdateAuthorCommandHandler::handleAsync(const UpdateAuthorCommand &request)
{
    QtConcurrent::run([=]() {
        auto authorResult = handleImpl(request);
        emit authorUpdated(authorResult);
    }).onFailed([=](const std::exception &ex) {
        qDebug() << "Error handling UpdateAuthorCommand asynchronously:" << ex.what();
        emit authorUpdated(Result<AuthorDTO>(Error(this, Error::Critical, "Unknown error", ex.what())));
    });
}

Result<AuthorDTO> UpdateAuthorCommandHandler::handleImpl(const UpdateAuthorCommand &request)
{
    qDebug() << "UpdateAuthorCommandHandler::handleImpl called with id" << request.req.uuid();

    // validate:
    auto validator = UpdateAuthorCommandValidator(m_repository);
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
    auto authorResult = m_repository->update(std::move(author));
    if (authorResult.hasError())
    {
        return Result<AuthorDTO>(authorResult.error());
    }

    // map
    auto authorDto = AutoMapper::AutoMapper::map<AuthorDTO>(authorResult.value());

    qDebug() << "UpdateAuthorCommandHandler::handleImpl done";

    return Result<AuthorDTO>(authorDto);
}
