#include "get_author_list_request_handler.h"
#include "QtConcurrent/qtconcurrentrun.h"
#include "automapper/automapper.h"
#include "persistence/interface_author_repository.h"

using namespace Application::Features::Author::Queries;

GetAuthorListRequestHandler::GetAuthorListRequestHandler(QSharedPointer<InterfaceAuthorRepository> repository)
    : Handler(), m_repository(repository)
{
}

Result<QList<AuthorDTO>> GetAuthorListRequestHandler::handle()
{
    qDebug() << "GetAuthorListRequestHandler::handle called";

    Result<QList<AuthorDTO>> result;

    try
    {
        result = handleImpl();
        emit getAuthorListReplied(result);
    }
    catch (const std::exception &ex)
    {
        result = Result<QList<AuthorDTO>>(Error(this, Error::Critical, "Unknown error", ex.what()));
        qDebug() << "Error handling GetAuthorListRequest:" << ex.what();
        emit getAuthorListReplied(result);
    }

    return result;
}

void GetAuthorListRequestHandler::handleAsync()
{
    qDebug() << "GetAuthorListRequestHandler::handleAsync called";

    QtConcurrent::run([=]() {
        auto authorListResult = handleImpl();
        emit getAuthorListReplied(authorListResult);
    }).onFailed([=](const std::exception &ex) {
        qDebug() << "Error handling GetAuthorListRequest asynchronously:" << ex.what();
        emit getAuthorListReplied(Result<QList<AuthorDTO>>(Error(this, Error::Critical, "Unknown error", ex.what())));
    });
}

Result<QList<AuthorDTO>> GetAuthorListRequestHandler::handleImpl()
{
    qDebug() << "GetAuthorListRequestHandler::handleImpl called";

    // do
    auto authorResult = m_repository->getAll();

    if (authorResult.isError())
    {
        return Result<QList<AuthorDTO>>(authorResult.error());
    }

    // map
    QList<AuthorDTO> dtoList;

    for (const Domain::Author &author : authorResult.value())
    {
        auto dto = AutoMapper::AutoMapper::map<AuthorDTO>(author);
        dtoList.append(dto);
    }

    qDebug() << "GetAuthorListRequestHandler::handleImpl done";

    return Result<QList<AuthorDTO>>(dtoList);
}
