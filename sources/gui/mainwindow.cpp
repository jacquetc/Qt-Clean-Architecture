#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "author/author_controller.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto list = Presenter::Author::getAll();

    for (auto author : list.value())
    {
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
