#include "undo_controller.h"

using namespace Presenter;

QMutex UndoController::s_mutex;

UndoController::UndoController()
{
    m_undoStack.reset(new QUndoStack());
}

QUndoStack *UndoController::undoStack()
{
    QMutexLocker locker(&s_mutex);
    return m_undoStack.get();
}
UndoController *UndoController::s_instance = nullptr;

UndoController *UndoController::instance()
{
    if (!s_instance)
    {
        s_instance = new UndoController;
    }
    return s_instance;
}
