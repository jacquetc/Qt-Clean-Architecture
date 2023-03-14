#include "query_command.h"

using namespace Presenter::UndoRedo;

QueryCommand::QueryCommand(const QString &text) : UndoRedoCommand(text)
{
}

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
