#pragma once

#include "presenter_global.h"
#include <QMutex>
#include <QUndoStack>

namespace Presenter
{

class SKRPRESENTEREXPORT UndoController
{
  public:
    static UndoController *instance();

    QUndoStack *undoStack();

  private:
    explicit UndoController();
    static UndoController *s_instance;
    QScopedPointer<QUndoStack> m_undoStack;
    static QMutex s_mutex;
};
} // namespace Presenter
