#include "remove_author_command_handler.h"
#include "automapper/automapper.h"
#include "persistence/interface_author_repository.h"

using namespace Contracts::DTO::Author;
using namespace Contracts::Persistence;
using namespace Contracts::CQRS::Author::Commands;
using namespace Application::Features::Author::Commands;

RemoveAuthorCommandHandler::RemoveAuthorCommandHandler(QSharedPointer<InterfaceAuthorRepository> repository)
    : Handler(), m_repository(repository)
{
}

Result<AuthorDTO> RemoveAuthorCommandHandler::handle(const RemoveAuthorCommand &request)
{
    Result<AuthorDTO> result;

    try
    {
        result = handleImpl(request);
    }
    catch (const std::exception &ex)
    {
        result = Result<AuthorDTO>(Error(this, Error::Critical, "Unknown error", ex.what()));
        qDebug() << "Error handling RemoveAuthorCommand:" << ex.what();
    }

    return result;
}

Result<AuthorDTO> RemoveAuthorCommandHandler::handleImpl(const RemoveAuthorCommand &request)
{
    Result<Domain::Author> authorResult = m_repository->get(request.id);
    if (authorResult.hasError())
    {
        qDebug() << "Error getting author from repository:" << authorResult.error().message();
        return Result<AuthorDTO>(authorResult.error());
    }

    auto deleteResult = m_repository->remove(std::move(authorResult.value()));
    if (deleteResult.hasError())
    {
        qDebug() << "Error deleting author from repository:" << deleteResult.error().message();
        return Result<AuthorDTO>(deleteResult.error());
    }

    // map
    auto dto = AutoMapper::AutoMapper::map<AuthorDTO>(deleteResult.value());

    qDebug() << "Author removed:" << dto.uuid();

    return Result<AuthorDTO>(dto);
}