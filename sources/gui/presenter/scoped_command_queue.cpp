#include "scoped_command_queue.h"

using namespace Presenter;

QHash<ScopedCommandQueue::Scope, QQueue<QUndoCommand *>> ScopedCommandQueue::s_commandQueue;
QHash<ScopedCommandQueue::Scope, QUndoCommand *> ScopedCommandQueue::s_currentCommand;
QMutex ScopedCommandQueue::s_mutex;
QMutex ScopedCommandQueue::s_undoStackMutex;
QMutex ScopedCommandQueue::s_checkNextCommandMutex;
UndoController *ScopedCommandQueue::s_undoController;
