#pragma once

#include "dto/author/author_dto.h"
#include "presenter_global.h"
#include "result.h"

#include <QObject>

using namespace Contracts::DTO::Author;

namespace Presenter::Author::Private
{

class SKR_PRESENTER_EXPORT AuthorSignalBridge : public QObject
{
    Q_OBJECT
  public:
    AuthorSignalBridge(QObject *parent) : QObject(parent)
    {
    }

  signals:

    void getAuthorReplied(Result<Contracts::DTO::Author::AuthorDTO> result);
    void getAuthorListReplied(Result<QList<Contracts::DTO::Author::AuthorDTO>> result);
    void authorCreated(Result<QUuid> result);
    void authorRemoved(Result<Contracts::DTO::Author::AuthorDTO> result);
    void authorUpdated(Result<Contracts::DTO::Author::AuthorDTO> result);
};
} // namespace Presenter::Author::Private
