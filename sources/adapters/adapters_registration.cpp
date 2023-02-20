#include "adapters_registration.h"

#include <QUndoStack>

AdaptersRegistration::AdaptersRegistration(QObject *parent) : QObject{parent}
{

    s_undoController.reset(Adapters::UndoController::instance());
}
