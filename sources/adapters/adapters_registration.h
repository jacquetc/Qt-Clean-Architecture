#ifndef ADAPTERSREGISTRATION_H
#define ADAPTERSREGISTRATION_H

#include "undo_controller.h"

#include <QObject>

class AdaptersRegistration : public QObject
{
    Q_OBJECT
  public:
    explicit AdaptersRegistration(QObject *parent);

  signals:

  private:
    QScopedPointer<Adapters::UndoController> s_undoController;
};

#endif // ADAPTERSREGISTRATION_H
