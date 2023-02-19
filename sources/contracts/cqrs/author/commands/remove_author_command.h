#ifndef REMOVE_AUTHOR_COMMAND_H
#define REMOVE_AUTHOR_COMMAND_H

#include <QUuid>


namespace Contracts::CQRS::Author::Commands {
class RemoveAuthorCommand {
public:

    RemoveAuthorCommand() {}

    QUuid id;
};
} // namespace Contracts::CQRS::Author::Commands

#endif // REMOVE_AUTHOR_COMMAND_H
