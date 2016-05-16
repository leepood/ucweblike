#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "wtfwidget.h"
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->init();
//    setFixedSize(300,300);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init(){
    WTFWidget* widget = new WTFWidget(this);
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(widget, 0, 0);
    setLayout(layout);
}
