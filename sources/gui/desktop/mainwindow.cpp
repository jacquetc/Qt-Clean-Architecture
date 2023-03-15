#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "author/author_controller.h"
#include "system/system_controller.h"

using namespace Presenter::Author;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Contracts::DTO::System::LoadSystemDTO dto;
    dto.setFileName(QUrl::fromLocalFile("/Users/cyril/Documents/test_clean.skrib"));

    //    Result<void> result = Presenter::System::loadSystem(dto);
    //    if (!result)
    //    {
    //        qDebug() << result.error().message() << result.error().data();
    //    }
    auto list = AuthorController::instance()->getAll();

    ui->redoToolButton->setDefaultAction(
        Presenter::UndoRedo::ThreadedUndoRedoSystem::instance()->createRedoAction(this));
    ui->undoToolButton->setDefaultAction(
        Presenter::UndoRedo::ThreadedUndoRedoSystem::instance()->createUndoAction(this));

    for (const auto &author : list.value())
    {
        auto *item = new QListWidgetItem(ui->listWidget);
        item->setText(author.name());
    }
    auto authorController = AuthorController::instance();

    connect(ui->addAsyncPushButton, &QPushButton::clicked, this, [=]() {
        Contracts::DTO::Author::CreateAuthorDTO dto("test", QUuid::createUuid());

        authorController->createAsync(dto);
    });

    connect(authorController, &AuthorController::authorCreated, this, [this](Result<QUuid> result) {
        if (!result)
        {
            qDebug() << result.error().message() << result.error().data();
        }
        auto author = AuthorController::instance()->get(result.value());
        if (!author)
        {
            qDebug() << author.error().message() << author.error().data();
        }
        auto *item = new QListWidgetItem(ui->listWidget);
        item->setText(author.value().name());
        item->setData(Qt::UserRole, author.value().uuid());
    });

    connect(ui->removePushButton, &QPushButton::clicked, this, [this]() {
        QListWidgetItem *item = ui->listWidget->item(0);
        auto authorController = AuthorController::instance();

        authorController->removeAsync(item->data(Qt::UserRole).toUuid());
    });

    connect(authorController, &AuthorController::authorRemoved, this,
            [this](Result<Contracts::DTO::Author::AuthorDTO> result) {
                if (!result)
                {
                    qDebug() << result.error().message() << result.error().data();
                }

                for (int i = 0; i < ui->listWidget->count(); i++)
                {
                    if (ui->listWidget->item(i)->data(Qt::UserRole).toUuid() == result.value().uuid())
                    {
                        ui->listWidget->takeItem(i);
                        break;
                    }
                }
            });
}

MainWindow::~MainWindow()
{
    delete ui;
}
