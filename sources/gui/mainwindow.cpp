#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "author/author_controller.h"
#include "system/system_controller.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Contracts::DTO::System::LoadSystemDTO dto;
    dto.setFileName(QUrl::fromLocalFile("/Users/cyril/Documents/test_clean.skrib"));

    Result<void *> result = Presenter::System::loadSystem(dto);
    if (!result)
    {
        qDebug() << result.error().message() << result.error().data();
    }
    auto list = Presenter::Author::getAll();

    for (const auto &author : list.value())
    {
        auto *item = new QListWidgetItem(ui->listWidget);
        item->setText(author.name());
    }

    connect(ui->addPushButton, &QPushButton::clicked, this, [this]() {
        Contracts::DTO::Author::CreateAuthorDTO dto("test", QUuid::createUuid());

        auto result = Presenter::Author::create(dto);
        if (!result)
        {
            qDebug() << result.error().message() << result.error().data();
        }
        auto author = Presenter::Author::get(result.value());
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
        auto result = Presenter::Author::remove(item->data(Qt::UserRole).toUuid());
        if (!result)
        {
            qDebug() << result.error().message() << result.error().data();
        }

        ui->listWidget->takeItem(0);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
