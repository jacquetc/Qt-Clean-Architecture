#ifndef UNDOCONTROLLER_H
#define UNDOCONTROLLER_H

#include <QUndoStack>

namespace Adapters
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
} // namespace Adapters
#endif // UNDOCONTROLLER_H
