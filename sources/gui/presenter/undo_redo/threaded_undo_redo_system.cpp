#include "threaded_undo_redo_system.h"

using namespace Presenter::UndoRedo;

ThreadedUndoRedoSystem *ThreadedUndoRedoSystem::m_instance = nullptr;

ThreadedUndoRedoSystem::ThreadedUndoRedoSystem(QObject *parent) : QObject(parent)
{
    QMutexLocker locker(&m_mutex);
    // singleton
    if (m_instance)
        return;
    m_instance = this;

    // Create a new UndoRedoSystem instance
    m_undoRedoSystem = new UndoRedoSystem();

    // Move the UndoRedoSystem to a new thread
    m_thread = new QThread();
    m_undoRedoSystem->moveToThread(m_thread);

    // Connect the thread started signal to the startUndoRedoSystem slot
    connect(m_thread, &QThread::started, this, &ThreadedUndoRedoSystem::startUndoRedoSystem);

    // Start the thread
    m_thread->start();
}

ThreadedUndoRedoSystem::~ThreadedUndoRedoSystem()
{
    QMutexLocker locker(&m_mutex);
    // Stop the thread
    m_thread->quit();
    m_thread->wait();

    // Delete the UndoRedoSystem instance and the thread
    delete m_undoRedoSystem;
    delete m_thread;
}

ThreadedUndoRedoSystem *ThreadedUndoRedoSystem::instance()
{
    if (!m_instance)
        qFatal("No ThreadedUndoRedoSystem instance found");
    return m_instance;
}

bool ThreadedUndoRedoSystem::canUndo() const
{
    QMutexLocker locker(&m_mutex);
    bool result = false;
    QMetaObject::invokeMethod(m_undoRedoSystem, "canUndo", Qt::QueuedConnection, Q_RETURN_ARG(bool, result));
    return result;
}

bool ThreadedUndoRedoSystem::canRedo() const
{
    QMutexLocker locker(&m_mutex);
    bool result = false;
    QMetaObject::invokeMethod(m_undoRedoSystem, "canRedo", Qt::QueuedConnection, Q_RETURN_ARG(bool, result));
    return result;
}

void ThreadedUndoRedoSystem::undo()
{
    QMutexLocker locker(&m_mutex);
    QMetaObject::invokeMethod(m_undoRedoSystem, "undo", Qt::QueuedConnection);
}

void ThreadedUndoRedoSystem::redo()
{
    QMutexLocker locker(&m_mutex);
    QMetaObject::invokeMethod(m_undoRedoSystem, "redo", Qt::QueuedConnection);
}

void ThreadedUndoRedoSystem::push(UndoRedoCommand *command, const UndoRedoCommand::Scope &scope)
{
    QMutexLocker locker(&m_mutex);
    command->moveToThread(m_undoRedoSystem->thread());
    QMetaObject::invokeMethod(m_undoRedoSystem, "push", Qt::QueuedConnection, Q_ARG(UndoRedoCommand *, command),
                              Q_ARG(UndoRedoCommand::Scope, scope));
}

void ThreadedUndoRedoSystem::startUndoRedoSystem()
{
    QMutexLocker locker(&m_mutex);
    // Connect the UndoRedoSystem's stateChanged signal to this class's stateChanged signal
    connect(m_undoRedoSystem, &UndoRedoSystem::stateChanged, this,
            &ThreadedUndoRedoSystem::onUndoRedoSystemStateChanged);
}

void ThreadedUndoRedoSystem::onUndoRedoSystemStateChanged()
{
    QMutexLocker locker(&m_mutex);
    // Emit the stateChanged signal
    emit stateChanged();
}
