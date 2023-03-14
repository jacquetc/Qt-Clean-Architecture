#pragma once

#include "contracts_global.h"
#include "dto/system/load_system_dto.h"
#include "result.h"

#include <QFile>

using namespace Contracts::DTO::System;

namespace Contracts::CQRS::Author::Validators
{
class SKRCONTRACTSEXPORT LoadSystemCommandValidator
{
  public:
    LoadSystemCommandValidator()
    {
    }

    Result<void *> validate(const LoadSystemDTO &dto) const
    {
        QUrl url = dto.fileName();

        if (!url.isValid())
        {
            return Result<void *>(Error("LoadSystemCommandValidator", Error::Critical, "invalid_filename"));
        }

        if (!url.isLocalFile() && url.scheme() != "qrc")
        {
            return Result<void *>(Error("LoadSystemCommandValidator", Error::Critical, "invalid_not_local"));
        }

        QString fileNameString;

        if (url.scheme() == "qrc")
        {
            fileNameString = url.toString(QUrl::RemoveScheme);
            fileNameString = ":" + fileNameString;
        }
        else if (url.path().at(2) == ':')
        { // means Windows
            fileNameString = url.path().remove(0, 1);
        }
        else
        {
            fileNameString = url.path();
        }

        QFile file(fileNameString);

        if (!file.exists())
        {

            return Result<void *>(Error("LoadSystemCommandValidator", Error::Critical, "absent_filename",
                                        fileNameString + " doesn't exist", fileNameString));
        }

        if (!file.open(QIODevice::ReadOnly))
        {
            return Result<void *>(Error("LoadSystemCommandValidator", Error::Critical, "readonly_filename",
                                        fileNameString + " can't be opened", fileNameString));
        }

        // Return that is Ok :
        return Result<void *>(nullptr);
    }
};
} // namespace Contracts::CQRS::Author::Validators