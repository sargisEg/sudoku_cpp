#include "scoreswindow.h"
#include "ui_scoreswindow.h"

#include <QDebug>
#include <dbhelper.h>
#include <player.h>
#include <vector>

ScoresWindow::ScoresWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ScoresWindow)
{
    ui->setupUi(this);
    this->move(200, 150);

}

ScoresWindow::~ScoresWindow()
{
    delete ui;
}

void ScoresWindow::initData()
{
    DbHelper dbHelper;
    std::vector<Player> players = dbHelper.getRecords();
    for (int i = 0; i < players.size(); ++i) {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(players[i].getName()));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(players[i].getDifficluty()));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(players[i].getTime()));
    }
}

void ScoresWindow::closeEvent(QCloseEvent *event)
{
    emit exit();
}
