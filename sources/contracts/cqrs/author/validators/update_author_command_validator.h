#ifndef UPDATE_AUTHOR_COMMAND_VALIDATOR_H
#define UPDATE_AUTHOR_COMMAND_VALIDATOR_H

#include "dto/author/update_author_dto.h"
#include "persistence/interface_author_repository.h"
#include "result.h"

using namespace Contracts::Persistence;
using namespace Contracts::DTO::Author;

namespace Contracts::CQRS::Author::Validators
{
class UpdateAuthorCommandValidator
{
  public:
    UpdateAuthorCommandValidator(InterfaceAuthorRepository *authorRepository) : m_repository(authorRepository)
    {
    }

    Result<void *> validate(const UpdateAuthorDTO &dto) const
    {

        Result<bool> exists = m_repository->exists(dto.uuid());
        if (!exists.value())
        {
            return Result<void *>(Error("UpdateAuthorCommandValidator", Error::Critical, "uuid_missing"));
        }
        //        if (!dto.relative().isNull())
        //        {
        //            return Result<void *>(Error("CreateAuthorValidator", Error::Critical, "project_uuid_missing"));
        //        }

        // Return that is Ok :
        return Result<void *>(nullptr);
    }

  private:
    InterfaceAuthorRepository *m_repository;
};
} // namespace Contracts::CQRS::Author::Validators

#endif // UPDATE_AUTHOR_COMMAND_VALIDATOR_H
