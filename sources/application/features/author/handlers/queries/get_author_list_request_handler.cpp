#include "get_author_list_request_handler.h"
#include "QtConcurrent/qtconcurrenttask.h"
#include "automapper/automapper.h"
#include "persistence/interface_author_repository.h"

using namespace Application::Features::Author::Queries;

GetAuthorListRequestHandler::GetAuthorListRequestHandler(InterfaceRepositories *repositories)
    : Handler(), m_repositories(repositories)
{
}

Result<QList<AuthorDTO>> GetAuthorListRequestHandler::handle()
{
    return waitInEventLoop<QList<AuthorDTO>>(QtConcurrent::task([this]() {
                                                 // get repository
                                                 InterfaceAuthorRepository *repository =
                                                     dynamic_cast<InterfaceAuthorRepository *>(
                                                         m_repositories->get(InterfaceRepositories::Entities::Author));

                                                 // do
                                                 auto authorResult = repository->getAll();

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

                                                 return Result<QList<AuthorDTO>>(dtoList);
                                             }).spawn());
}
