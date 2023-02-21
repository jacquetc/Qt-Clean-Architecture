#pragma once
#include "presenter_global.h"
#include "undo_controller.h"

#include <QObject>

class SKRPRESENTEREXPORT PresenterRegistration : public QObject
{
    Q_OBJECT
  public:
    explicit PresenterRegistration(QObject *parent);

  signals:

  private:
    QScopedPointer<Presenter::UndoController> s_undoController;
};
