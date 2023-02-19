#ifndef REMOVE_AUTHOR_COMMAND_H
#define REMOVE_AUTHOR_COMMAND_H

#include "dto/author/update_author_dto.h"


namespace Contracts::CQRS::Author::Commands {
class UpdateAuthorCommand {
public:

    UpdateAuthorCommand() {}

    Contracts::DTO::Author::UpdateAuthorDTO req;
};
} // namespace Contracts::CQRS::Author::Commands

#endif // REMOVE_AUTHOR_COMMAND_H
