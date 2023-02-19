#ifndef CREATE_AUTHOR_COMMAND_H
#define CREATE_AUTHOR_COMMAND_H

#include "dto/author/create_author_dto.h"


namespace Contracts::CQRS::Author::Commands {
class CreateAuthorCommand {
public:

    CreateAuthorCommand() {}

    Contracts::DTO::Author::CreateAuthorDTO req;
};
} // namespace Contracts::CQRS::Author::Commands

#endif // CREATE_AUTHOR_COMMAND_H
