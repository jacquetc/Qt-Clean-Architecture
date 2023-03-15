#include "undo_redo_system.h"
#include "undo_redo/query_command.h"

#include <QEventLoop>

using namespace Presenter::UndoRedo;

UndoRedoSystem::UndoRedoSystem(QObject *parent) : QObject(parent), m_currentIndex(-1), m_undoLimit(10)
{
}
void UndoRedoSystem::run()
{
    // Create an event loop for the thread
    QEventLoop eventLoop;

    // Connect the thread started signal to the event loop quit slot
    connect(this, &UndoRedoSystem::finished, &eventLoop, &QEventLoop::quit);

    // Execute the event loop
    eventLoop.exec();
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

void UndoRedoSystem::clear()
{
    // Clear the general command queue, not the scoped command queue
    this->m_generalCommandQueue.clear();
    // Set the current index to -1
    this->m_currentIndex = -1;
    // Emit the stateChanged signal
    emit stateChanged();
}
void UndoRedoSystem::setUndoLimit(int limit)
{
    m_undoLimit = limit;
    // Remove excess commands from the general command queue if necessary
    while (m_generalCommandQueue.size() > m_undoLimit)
    {
        m_generalCommandQueue.dequeue();
        m_currentIndex--;
    }
    // Emit the stateChanged signal
    emit stateChanged();
}

int UndoRedoSystem::undoLimit() const
{
    return m_undoLimit;
}

QString UndoRedoSystem::undoText() const
{
    if (m_currentIndex >= 0)
    {
        return m_generalCommandQueue[m_currentIndex]->text();
    }
    else
    {
        return QString();
    }
}

QString UndoRedoSystem::redoText() const
{
    if (m_currentIndex < m_generalCommandQueue.size() - 1)
    {
        return m_generalCommandQueue[m_currentIndex + 1]->text();
    }
    else
    {
        return QString();
    }
}

void UndoRedoSystem::onCommandFinished()
{

    auto command = dynamic_cast<UndoRedoCommand *>(QObject::sender());

    const UndoRedoCommand::Scope &scope = command->scope();

    // Set the current command to nullptr
    m_currentCommandHash.insert(scope, QSharedPointer<UndoRedoCommand>());

    if (command->obsolete())
    {
        m_generalCommandQueue.removeLast();
    }

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
    if (qSharedPointerDynamicCast<QueryCommand>(m_scopedCommandQueueHash[scope].head()).isNull())
    {
        // Remove any redo commands that are after the current index
        m_generalCommandQueue.erase(m_generalCommandQueue.begin() + m_currentIndex + 1, m_generalCommandQueue.end());

        // Add the new command to the end of the list

        m_generalCommandQueue.enqueue(m_scopedCommandQueueHash[scope].head());
        // Increment the current index
        m_currentIndex++;

        // Remove excess commands from the general command queue if necessary
        while (m_generalCommandQueue.size() > m_undoLimit)
        {
            m_generalCommandQueue.dequeue();
            m_currentIndex--;
        }
    }

    // Dequeue the next command
    QQueue<QSharedPointer<UndoRedoCommand>> &queue = m_scopedCommandQueueHash[scope];
    QSharedPointer<UndoRedoCommand> command = queue.dequeue();

    m_currentCommandHash.insert(scope, command);

    // Connect the finished signal to the onCommandFinished slot
    connect(command.data(), &UndoRedoCommand::finished, this, &UndoRedoSystem::onCommandFinished, Qt::UniqueConnection);

    // Execute the command asynchronously
    command->asyncRedo();
}
