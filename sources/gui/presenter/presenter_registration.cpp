#include "presenter_registration.h"
#include "author/author_controller.h"
#include "repositories/repository_provider.h"

using namespace Presenter;
PresenterRegistration::PresenterRegistration(QObject *parent) : QObject{parent}
{

    auto *undoRedoSystem = new UndoRedo::ThreadedUndoRedoSystem(this);

    s_undoRedoSystem.reset(undoRedoSystem);
    auto repository_provider = Repository::RepositoryProvider::instance();

    // initialize controllers
    new Author::AuthorController(repository_provider);
}
