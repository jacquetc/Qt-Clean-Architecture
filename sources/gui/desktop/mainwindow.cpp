#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "author/author_controller.h"
#include "system/system_controller.h"

#include <QMessageBox>

using namespace Presenter::Author;
using namespace Presenter::System;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // disable all:
    ui->saveSystemPushButton->setEnabled(false);
    ui->addAsyncPushButton->setEnabled(false);
    ui->removePushButton->setEnabled(false);

    // error handling :

    connect(ThreadedUndoRedoSystem::instance(), &ThreadedUndoRedoSystem::errorSent, this, [=](Error error) {
        QMessageBox *box = new QMessageBox(this);
        switch (error.status())
        {
        case Error::Status::Warning:
            box->setIcon(QMessageBox::Icon::Warning);
            box->setText(tr("Warning: %1").arg(error.message()));
            box->setDetailedText(error.code());
            break;
        case Error::Status::Critical:
            box->setIcon(QMessageBox::Icon::Critical);
            box->setText(tr("Critical: %1").arg(error.message()));
            box->setDetailedText(error.code());
            break;
        case Error::Status::Fatal:
            box->setIcon(QMessageBox::Icon::NoIcon);
            box->setText(tr("Fatal: %1").arg(error.message()));
            box->setDetailedText(error.code());
            break;
        default:
            break;
        }
        box->setModal(true);
        box->show();
    });

    // system :

    auto systemController = SystemController::instance();

    connect(ui->loadSystemPushButton, &QPushButton::clicked, this, [=]() {
        Contracts::DTO::System::LoadSystemDTO dto;
        dto.setFileName(QUrl("qrc:/test_clean.skrib"));

        SystemController::loadSystem(dto);
    });

    connect(systemController, &SystemController::systemLoaded, this, [this](Result<void> result) {
        if (!result)
        {
            qDebug() << result.error().message() << result.error().data();
            return;
        }
        ui->addAsyncPushButton->setEnabled(true);
        ui->removePushButton->setEnabled(true);

        auto list = AuthorController::instance()->getAll();

        for (const auto &author : list.value())
        {
            auto *item = new QListWidgetItem(ui->listWidget);
            item->setText(author.name());
        }
    });

    // undo redo buttons:

    ui->redoToolButton->setDefaultAction(
        Presenter::UndoRedo::ThreadedUndoRedoSystem::instance()->createRedoAction(this));
    ui->undoToolButton->setDefaultAction(
        Presenter::UndoRedo::ThreadedUndoRedoSystem::instance()->createUndoAction(this));

    // add / remove buttons:

    auto authorController = AuthorController::instance();

    connect(ui->addAsyncPushButton, &QPushButton::clicked, this, [=]() {
        Contracts::DTO::Author::CreateAuthorDTO dto("test", QUuid::createUuid());

        authorController->createAsync(dto);
    });

    connect(authorController, &AuthorController::authorCreated, this, [this](Result<QUuid> result) {
        if (!result)
        {
            qDebug() << result.error().message() << result.error().data();
            return;
        }
        auto author = AuthorController::instance()->get(result.value());
        if (!author)
        {
            qDebug() << author.error().message() << author.error().data();
            return;
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
                    return;
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
