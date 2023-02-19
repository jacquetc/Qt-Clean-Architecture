#ifndef GET_AUTHOR_REQUEST_WITH_DETAILS_H
#define GET_AUTHOR_REQUEST_WITH_DETAILS_H

#include <QUuid>

namespace Contracts::CQRS::Author::Requests
{
class GetAuthorRequest
{
  public:
    GetAuthorRequest()
    {
    }

    QUuid id;
};
} // namespace Contracts::CQRS::Author::Requests

#endif // GET_AUTHOR_REQUEST_H
