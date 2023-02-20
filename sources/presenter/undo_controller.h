#pragma once

#include <QUndoStack>

namespace Presenter
{

class UndoController
{
  public:
    static UndoController *instance();

    QUndoStack *undoStack();

  private:
    explicit UndoController();
    static UndoController *s_instance;
    QScopedPointer<QUndoStack> m_undoStack;
};
} // namespace Presenter
