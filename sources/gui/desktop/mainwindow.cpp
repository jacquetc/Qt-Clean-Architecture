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

    //    Result<void *> result = Presenter::System::loadSystem(dto);
    //    if (!result)
    //    {
    //        qDebug() << result.error().message() << result.error().data();
    //    }
    auto list = AuthorController::instance()->getAll();

    for (const auto &author : list.value())
    {
        auto *item = new QListWidgetItem(ui->listWidget);
        item->setText(author.name());
    }

    connect(ui->addAsyncPushButton, &QPushButton::clicked, this, [this]() {
        Contracts::DTO::Author::CreateAuthorDTO dto("test", QUuid::createUuid());
        auto authorController = AuthorController::instance();

        connect(
            authorController, &AuthorController::authorCreated, this,
            [this](Result<QUuid> result) {
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
            },
            Qt::SingleShotConnection);

        authorController->createAsync(dto);
    });

    connect(ui->removePushButton, &QPushButton::clicked, this, [this]() {
        QListWidgetItem *item = ui->listWidget->item(0);
        auto authorController = AuthorController::instance();

        connect(
            authorController, &AuthorController::authorRemoved, this,
            [this](Result<Contracts::DTO::Author::AuthorDTO> result) {
                if (!result)
                {
                    qDebug() << result.error().message() << result.error().data();
                }

                ui->listWidget->takeItem(0);
            },
            Qt::SingleShotConnection);

        authorController->removeAsync(item->data(Qt::UserRole).toUuid());
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
