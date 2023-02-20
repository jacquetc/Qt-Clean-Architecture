#pragma once
#include "undo_controller.h"

#include <QObject>

class PresenterRegistration : public QObject
{
    Q_OBJECT
  public:
    explicit PresenterRegistration(QObject *parent);

  signals:

  private:
    QScopedPointer<Presenter::UndoController> s_undoController;
};
