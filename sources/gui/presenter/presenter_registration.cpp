#include "presenter_registration.h"
#include "author/author_controller.h"
#include "repositories/repository_provider.h"
#include "scoped_command_queue.h"

using namespace Presenter;
PresenterRegistration::PresenterRegistration(QObject *parent) : QObject{parent}
{
    new ScopedCommandQueue(this, Presenter::UndoController::instance());

    s_undoController.reset(Presenter::UndoController::instance());
    auto repository_provider = Repository::RepositoryProvider::instance();

    // initialize controllers
    new Author::AuthorController(repository_provider);
}
