#include "welcomewindow.h"
#include "ui_welcomewindow.h"
#include <QInputDialog>
#include <QDebug>

WelcomeWindow::WelcomeWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WelcomeWindow)
{
    ui->setupUi(this);
    this->move(200, 150);

    QPushButton *exitButton = ui->exitButton;
    QPushButton *scoreButton = ui->scoreButton;
    QPushButton *startButton = ui->startButton;

    connect(exitButton, &QPushButton::clicked, this, &QWidget::close);
    connect(scoreButton, &QPushButton::clicked, this, [this] {emit scores();});
    connect(startButton, &QPushButton::clicked, this, [this] {
        startButtonClick();
    });
}

WelcomeWindow::~WelcomeWindow()
{
    delete ui;
}

void WelcomeWindow::startButtonClick()
{
    QRadioButton *medium = ui->mediumRadioButton;
    QRadioButton *hard = ui->hardRadioButton;
    int diff = 0;

    if (medium->isChecked()) {
        diff = 1;
    }

    if (hard->isChecked()) {
        diff = 2;
    }
    QString name = QInputDialog::getText(this, "Sudoku", "Please enter your name");
    if (!name.isEmpty()) {
        emit start(diff, name);
    }
}
