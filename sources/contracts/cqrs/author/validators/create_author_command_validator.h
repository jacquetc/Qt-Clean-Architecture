#pragma once

#include "contracts_global.h"
#include "dto/author/create_author_dto.h"
#include "persistence/interface_author_repository.h"
#include "result.h"

using namespace Contracts::Persistence;
using namespace Contracts::DTO::Author;

namespace Contracts::CQRS::Author::Validators
{
class SKRCONTRACTSEXPORT CreateAuthorCommandValidator
{
  public:
    CreateAuthorCommandValidator(InterfaceAuthorRepository *authorRepository) : m_repository(authorRepository)
    {
    }

    Result<void *> validate(const CreateAuthorDTO &dto) const
    {

        if (dto.relative().isNull())
        {
            return Result<void *>(Error("CreateAuthorCommandValidator", Error::Critical, "relative_uuid_missing"));
        }

        // Return that is Ok :
        return Result<void *>(nullptr);
    }

  private:
    InterfaceAuthorRepository *m_repository;
};
} // namespace Contracts::CQRS::Author::Validators
