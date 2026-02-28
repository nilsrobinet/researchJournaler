#include "mainwindow.h"
#include <QLabel>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Hello World - Qt Application");
    setGeometry(100, 100, 600, 400);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    QLabel *label = new QLabel("Hello World!", this);
    label->setStyleSheet("font-size: 48px; font-weight: bold; color: #0066cc;");
    label->setAlignment(Qt::AlignCenter);

    layout->addStretch();
    layout->addWidget(label);
    layout->addStretch();

    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
}

MainWindow::~MainWindow()
{
}
