#include "query_command.h"

using namespace Presenter::UndoRedo;

/*!
 * \class QueryCommand
 * \brief The QueryCommand class
 * \ingroup Presenter::UndoRedo
 *
 * Invisible command dedicated to queries. QueryCommands will not be stored in UndoRedoSystem as done with
 * UndoRedoCommand, but they will run asynchronously respecting the execution queue of its scope.
 */

/*!
 * \brief Constructs a QueryCommand instance with the specified text for logging purposes.
 * \param text A QString representing the message for logging purposes.
 */
QueryCommand::QueryCommand(const QString &text) : UndoRedoCommand(text)
{
}

/*!
 * \brief Sets the function to be executed asynchronously when "redo()" is called in UndoRedoSystem.
 * \param function A std::function<void()> representing the function to be executed.
 */
void QueryCommand::setQueryFunction(const std::function<void()> &function)
{
    m_queryFunction = function;
}

void QueryCommand::undo()
{
    // Nothing to undo
}

void QueryCommand::redo()
{
    if (m_queryFunction)
    {
        m_queryFunction();
    }
}
