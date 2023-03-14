#include "undo_redo_system.h"
#include "undo_redo/query_command.h"

using namespace Presenter::UndoRedo;

UndoRedoSystem::UndoRedoSystem(QObject *parent) : QObject(parent), m_currentIndex(-1)
{
}

bool UndoRedoSystem::canUndo() const
{
    return m_currentIndex >= 0 && !m_generalCommandQueue[m_currentIndex]->isRunning();
}

bool UndoRedoSystem::canRedo() const
{
    return m_currentIndex < m_generalCommandQueue.size() - 1 && !m_generalCommandQueue[m_currentIndex + 1]->isRunning();
}

void UndoRedoSystem::undo()
{
    if (canUndo())
    {
        m_generalCommandQueue[m_currentIndex--]->asyncUndo();
        emit stateChanged();
    }
}

void UndoRedoSystem::redo()
{
    if (canRedo())
    {
        m_generalCommandQueue[++m_currentIndex]->asyncRedo();
        emit stateChanged();
    }
}

void UndoRedoSystem::push(UndoRedoCommand *command, const UndoRedoCommand::Scope &scope)
{
    command->setParent(this);

    QQueue<QSharedPointer<UndoRedoCommand>> &queue = m_scopedCommandQueueHash[scope];
    // Enqueue the command
    queue.enqueue(QSharedPointer<UndoRedoCommand>(command));

    // If the system is not currently executing a command, start executing the next command
    if (!m_currentCommandHash[scope])
    {
        executeNextCommand(scope);
    }

    // Emit the stateChanged signal
    emit stateChanged();
}

void UndoRedoSystem::onCommandFinished()
{

    auto command = dynamic_cast<UndoRedoCommand *>(QObject::sender());
    const UndoRedoCommand::Scope &scope = command->scope();
    // Set the current command to nullptr
    m_currentCommandHash.insert(scope, QSharedPointer<UndoRedoCommand>());

    // If there are commands in the queue, execute the next one
    if (!m_scopedCommandQueueHash[scope].isEmpty())
    {
        executeNextCommand(scope);
    }

    // Emit the stateChanged signal
    emit stateChanged();
}

void UndoRedoSystem::executeNextCommand(const UndoRedoCommand::Scope &scope)
{
    // keep in store only the true UndoRedoCommands, not QueryCommand
    if (qSharedPointerCast<QueryCommand>(m_currentCommandHash[scope]).isNull())
    {
        // Remove any redo commands that are after the current index
        m_generalCommandQueue.erase(m_generalCommandQueue.begin() + m_currentIndex + 1, m_generalCommandQueue.end());

        // Add the new command to the end of the list

        m_generalCommandQueue.enqueue(m_currentCommandHash[scope]);
    }
    // Increment the current index
    m_currentIndex++;

    QQueue<QSharedPointer<UndoRedoCommand>> &queue = m_scopedCommandQueueHash[scope];

    QSharedPointer<UndoRedoCommand> command = queue.dequeue();

    // Dequeue the next command
    m_currentCommandHash.insert(scope, command);

    // Connect the finished signal to the onCommandFinished slot
    connect(command.data(), &UndoRedoCommand::finished, this, &UndoRedoSystem::onCommandFinished, Qt::UniqueConnection);

    // Execute the command asynchronously
    command->asyncRedo();
}
