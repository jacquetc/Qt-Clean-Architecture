#include "presenter_registration.h"

#include <QUndoStack>

PresenterRegistration::PresenterRegistration(QObject *parent) : QObject{parent}
{

    s_undoController.reset(Presenter::UndoController::instance());
}
